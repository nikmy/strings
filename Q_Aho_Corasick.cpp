#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>

namespace strings {

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

    void AddPattern(const std::basic_string<CharT>& pattern) {
        auto node = root_.get();
        for (size_t i = 0; i < pattern.length(); ++i) {
            auto sym = pattern[i];
            if (node->next.count(sym) == 0) {
                node->next.emplace(sym, NewNode(node, sym));
            }
            node = node->next[sym].get();
        }
        node->is_leaf = true;
        node->pattern_id = size_++;
        node->pattern_len = pattern.length();
    }

    decltype(auto) FindEntries(const std::basic_string<CharT>& text) {
        std::vector<std::vector<size_t>> entries(size_);
        auto cur = root_.get();
        for (size_t i = 0; i < text.length(); ++i) {
            cur = GetLink(cur, text[i]);
            auto node = cur;
            while (node != root_.get()) {
                if (node->is_leaf) {
                    entries[node->pattern_id].push_back(i + 1 - node->pattern_len);
                }
                node = GetUp(node);
            }
        }
        return entries;
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
        size_t pattern_id = SIZE_MAX;
        size_t pattern_len = 0;
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
    size_t n_patterns = 0;
    cin >> text >> n_patterns;

    auto searcher = strings::PatternsSearcher<char>();
    for (size_t i = 0; i < n_patterns; ++i) {
        std::basic_string<char> pattern;
        cin >> pattern;
        searcher.AddPattern(pattern);
    }

    auto entries_data = searcher.FindEntries(text);
    for (auto& entries : entries_data) {
        cout << entries.size() << ' ';
        for (auto& entry : entries) {
            cout << entry + 1 << ' ';
        }
        cout << '\n';
    }
}