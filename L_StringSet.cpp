#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

namespace strings {

namespace detail {

template <class CharT>
class Trie {
    struct Node;

public:
    template <class KeyT, class ValueT>
    using HashMap = std::unordered_map<KeyT, ValueT>;
    using NodePtr = std::unique_ptr<Node>;
    using SizeType = size_t;

    Trie() : root_(NewNode()), size_(0) {
    }

    bool AddSequence(const std::basic_string<CharT>& sequence) {
        auto node = root_.get();
        auto sym = sequence.begin();
        auto end = sequence.end();
        bool is_new = false;
        while (sym != end) {
            if (node->next.count(*sym) == 0) {
                node->next.emplace(*sym, NewNode());
                is_new = true;
            }
            node = node->next[*sym].get();
            ++sym;
        }
        if (is_new) {
            ++size_;
        }
        return is_new;
    }

    bool Contains(const std::basic_string<CharT>& sequence) const {
        auto node = root_.get();
        auto sym = sequence.begin();
        auto end = sequence.end();
        while (sym != end) {
            if (node->next.count(*sym) == 0) {
                return false;
            }
            node = node->next[*sym].get();
            ++sym;
        }
        return true;
    }

    SizeType GetSize() const {
        return size_;
    }

private:
    NodePtr root_;
    SizeType size_;

    struct Node {
        HashMap<CharT, NodePtr> next;
    };

    static NodePtr NewNode() {
        return std::make_unique<Node>();
    }
};

}  // namespace detail

using StringSet = detail::Trie<char>;

}  // namespace strings

using std::cin;
using std::cout;

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);

    strings::StringSet set;

    char cmd = '#';
    std::basic_string<char> s;
    while (true) {
        cin >> cmd;
        if (cmd == '#') {
            break;
        }

        cin >> s;
        if (cmd == '+') {
            set.AddSequence(s);
        } else {
            cout << (set.Contains(s) ? "YES\n" : "NO\n");
        }
    }
}