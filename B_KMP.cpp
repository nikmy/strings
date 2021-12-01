#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;

namespace strings {

using LengthT = size_t;

void FillPrefix(const std::string& str, std::vector<LengthT>& prefix_func) {
    prefix_func.assign(str.length(), 0);
    for (size_t i = 1; i < str.length(); ++i) {
        auto k = prefix_func[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = prefix_func[k - 1];
        }
        if (str[i] == str[k]) {
            ++k;
        }
        prefix_func[i] = k;
    }
}

namespace detail {

void KnuthMorrisPratt(const std::string& str, const std::string& pattern, std::vector<LengthT>& entries) {
    entries.clear();
    std::vector<LengthT> prefix_func;
    FillPrefix(pattern + "#" + str, prefix_func);
    for (size_t i = 0; i < str.length(); ++i) {
        if (prefix_func[pattern.length() + i + 1] == pattern.length()) {
            entries.push_back(i + 1 - pattern.length());
        }
    }
}

}  // namespace detail

}  // namespace strings

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    std::string str;
    std::string pattern;
    std::vector<size_t> entries;
    cin >> str >> pattern;
    strings::detail::KnuthMorrisPratt(str, pattern, entries);
    for (auto e : entries) {
        cout << e << '\n';
    }
}