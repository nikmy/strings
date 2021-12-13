#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

using TPhoneNumber = std::basic_string<char>;

class PhonesTrie {
    struct Node;

public:
    template <class KeyT, class ValueT>
    using HashMap = std::unordered_map<KeyT, ValueT>;
    using NodePtr = std::unique_ptr<Node>;

    explicit PhonesTrie(size_t n_digits) : root_(NewNode()), max_n_digits_(n_digits) {
    }

    void AddCity(const TPhoneNumber& phone, const std::string& city) {
        cities_.push_back(city);
        if (phone.length() > max_n_digits_) {
            phones_count_.push_back(0);
            return;
        }
        size_t n_zeros = max_n_digits_ - phone.length();
        int64_t n_phones = 1;
        while (n_zeros-- > 0) {
            n_phones *= 10;
        }
        phones_count_.push_back(n_phones);
        auto city_id = static_cast<int64_t>(cities_.size() - 1);

        Node* node = root_.get();
        for (size_t i = 0; i < phone.length(); ++i) {
            auto digit = phone[i];
            if (node->next.count(digit) == 0) {
                node->next.emplace(digit, NewNode());
            }
            node = node->next[digit].get();
        }
        node->city_id = city_id;
    }

    void PrintCityPhoneCounts() {
        ReducePhoneCounts(root_.get());
        for (size_t i = 0; i < cities_.size(); ++i) {
            std::cout << cities_[i] << ' ' << phones_count_[i] << '\n';
        }
    }

private:
    NodePtr root_;
    std::vector<std::string> cities_;
    std::vector<int64_t> phones_count_;
    size_t max_n_digits_;

    struct Node {
        HashMap<char, NodePtr> next;
        int64_t city_id;

        Node() : next(), city_id(-1) {
        }
    };

    static NodePtr NewNode() {
        return std::make_unique<Node>();
    }

    int64_t ReducePhoneCounts(Node* node) {
        int64_t delta = 0;
        bool is_terminal = (node->city_id != -1);
        if (is_terminal) {
            delta = phones_count_[node->city_id];
        }
        for (auto& pair : node->next) {
            Node* next = pair.second.get();
            auto to_reduce = ReducePhoneCounts(next);
            if (is_terminal) {
                phones_count_[node->city_id] -= to_reduce;
            } else {
                delta += to_reduce;
            }
        }
        return delta;
    }
};

using std::cin;
using std::cout;

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n_cities = 0;
    size_t n_digits = 0;
    cin >> n_cities >> n_digits;
    PhonesTrie phones_data(n_digits);
    for (size_t i = 0; i < n_cities; ++i) {
        std::string city;
        TPhoneNumber phone;
        cin >> phone >> city;
        phones_data.AddCity(phone, city);
    }
    phones_data.PrintCityPhoneCounts();
}