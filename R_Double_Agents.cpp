#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <algorithm>

namespace strings {

struct Bounds {
    size_t left;
    size_t right;
};

template <class CharT = char>
class PatternsSearcher {
    struct Node;

public:
    template <class KeyT, class ValueT>
    using HashMap = std::map<KeyT, ValueT>;
    using NodePtr = std::unique_ptr<Node>;
    using SizeType = size_t;

    PatternsSearcher() : root_(NewNode()), size_(0) {
    }

    void AddPattern(const std::basic_string<CharT>& pattern, const Bounds& bounds) {
        auto node = root_.get();
        for (size_t i = 0; i < pattern.length(); ++i) {
            auto sym = pattern[i];
            if (node->next.count(sym) == 0) {
                node->next.emplace(sym, NewNode(node, sym));
            }
            node = node->next[sym].get();
        }
        node->is_leaf = true;
        node->pattern_ids.push_back(size_++);
        node->pattern_lens.push_back(pattern.length());
        node->pattern_bounds.push_back(bounds);
    }

    decltype(auto) FindEntries(const std::basic_string<CharT>& text) {
        std::vector<bool> has_entry(size_);
        auto cur = root_.get();
        for (size_t i = 0; i < text.length(); ++i) {
            cur = GetLink(cur, text[i]);
            auto node = cur;
            while (node != root_.get()) {
                if (node->is_leaf) {
                    for (size_t j = 0; j < node->pattern_ids.size(); ++j) {
                        if (node->pattern_bounds[j].left + node->pattern_lens[j] <= i + 1 &&
                            i <= node->pattern_bounds[j].right) {
                            has_entry[node->pattern_ids[j]] = true;
                        }
                    }
                }
                node = GetUp(node);
            }
        }
        return has_entry;
    }

private:
    NodePtr root_;
    SizeType size_;

    struct Node {
        HashMap<CharT, NodePtr> next;
        HashMap<CharT, Node*> go;
        Node* suf_link = nullptr;
        Node* up = nullptr;
        bool is_leaf = false;
        std::vector<size_t> pattern_ids;
        std::vector<size_t> pattern_lens;
        std::vector<Bounds> pattern_bounds;
        Node* prev;
        CharT prev_sym;

        Node(Node* prev, CharT prev_sym) : prev(prev), prev_sym(prev_sym) {
        }
    };

    static NodePtr NewNode(Node* prev = nullptr, CharT prev_sym = '\0') {
        return std::make_unique<Node>(prev, prev_sym);
    }

    Node* GetLink(Node* node, CharT sym) {
        if (!node->go.count(sym)) {
            if (node->next.count(sym)) {
                node->go[sym] = node->next[sym].get();
            } else if (node == root_.get()) {
                node->go[sym] = root_.get();
            } else {
                node->go[sym] = GetLink(GetSufLink(node), sym);
            }
        }
        return node->go[sym];
    }

    Node* GetSufLink(Node* node) {
        if (node->suf_link == nullptr) {
            if (node == root_.get() || node->prev == root_.get()) {
                node->suf_link = root_.get();
            } else {
                node->suf_link = GetLink(GetSufLink(node->prev), node->prev_sym);
            }
        }
        return node->suf_link;
    }

    Node* GetUp(Node* node) {
        if (node->up == nullptr) {
            auto suf_link = GetSufLink(node);
            if (suf_link->is_leaf || suf_link == root_.get()) {
                node->up = suf_link;
            } else {
                node->up = GetUp(suf_link);
            }
        }
        return node->up;
    }
};

}  // namespace strings

using std::cin;
using std::cout;

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);

    std::basic_string<char> text;
    size_t n_agents = 0;
    cin >> text >> n_agents;

    auto searcher = strings::PatternsSearcher<char>();
    for (size_t i = 0; i < n_agents; ++i) {
        std::basic_string<char> pattern;
        strings::Bounds bounds;
        cin >> bounds.left >> bounds.right >> pattern;
        --bounds.left;
        --bounds.right;
        searcher.AddPattern(pattern, bounds);
    }

    auto entries_data = searcher.FindEntries(text);
    for (size_t i = 0; i < n_agents; ++i) {
        cout << (entries_data[i] ? '+' : '-');
    }
}