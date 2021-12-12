#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <queue>

using Digit = char;
using Ticket = std::basic_string<Digit>;

struct Win {
    Ticket win_ticket;
    int64_t to_pay;
};

class LotteryTree {
    struct Node;

public:
    template <class KeyT, class ValueT>
    using HashMap = std::unordered_map<KeyT, ValueT>;
    using NodePtr = std::unique_ptr<Node>;

    LotteryTree() : root_(NewNode()) {
    }

    bool AddTicket(const Ticket& ticket) {
        auto node = root_.get();
        bool is_new = false;
        for (size_t i = 0; i < ticket.length(); ++i) {
            auto sym = ticket[i];
            if (node->next.count(sym) == 0) {
                node->next.emplace(sym, NewNode());
                is_new = true;
            }
            node->tree_size += 1;
            node = node->next[sym].get();
            if (i == ticket.length() - 1) {
                node->tree_size += 1;
            }
        }
        return is_new;
    }

    Win GetWin(int ticket_length, int n_digits, const std::vector<int64_t>& digit_award) {
        Win best = {"", INT64_MAX};
        std::queue<State> queue;
        queue.push({root_.get(), {"", 0}});
        queue.push({nullptr, {}});

        int cur_digit = 0;
        while (cur_digit < ticket_length) {
            auto[node, win] = queue.front();
            queue.pop();

            if (node == nullptr) {
                queue.push({nullptr, {}});
                ++cur_digit;
                continue;
            }

            bool finalized = false;
            for (char d = '0'; d < '0' + n_digits; ++d) {
                if (!node->next.count(d)) {
                    win.win_ticket.push_back(d);
                    FinalizeTicket(win.win_ticket, ticket_length);
                    if (win.to_pay < best.to_pay) {
                        best = win;
                    }
                    finalized = true;
                    break;
                }
            }
            if (finalized) {
                continue;
            }

            for (auto& next : node->next) {
                auto & [ next_digit, next_node ] = next;
                Ticket next_ticket = win.win_ticket + next_digit;
                int64_t next_to_pay = win.to_pay + next_node->tree_size * digit_award[cur_digit];
                if (cur_digit) {
                    next_to_pay -= next_node->tree_size * digit_award[cur_digit - 1];
                }
                queue.push({next_node.get(), {next_ticket, next_to_pay}});
            }
        }

        while (!queue.empty()) {
            auto[node, win] = queue.front();
            queue.pop();
            if (node == nullptr) {
                break;
            }
            if (win.to_pay < best.to_pay) {
                best = win;
            }
        }
        return best;
    }

private:
    NodePtr root_;

    struct Node {
        HashMap<Digit, NodePtr> next;
        int64_t tree_size;

        explicit Node(int64_t size) : next(), tree_size(size) {
        }
    };

    static NodePtr NewNode() {
        return std::make_unique<Node>(0);
    }

    static void FinalizeTicket(Ticket& ticket, size_t target_length) {
        ticket += Ticket(target_length - ticket.length(), '0');
    }

    struct State {
        Node* node;
        Win win;
    };
};

using std::cin;
using std::cout;

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n_tickets = 0;
    int ticket_length = 0;
    int n_digits = 0;
    cin >> n_tickets >> ticket_length >> n_digits;

    std::vector<int64_t> digit_award(ticket_length);
    for (auto& award : digit_award) {
        cin >> award;
    }

    LotteryTree lottery;
    for (int i = 0; i < n_tickets; ++i) {
        Ticket ticket;
        cin >> ticket;
        lottery.AddTicket(ticket);
    }

    auto win = lottery.GetWin(ticket_length, n_digits, digit_award);
    cout << win.win_ticket << '\n' << win.to_pay;
}