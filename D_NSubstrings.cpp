#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;

namespace strings {

using LengthT = size_t;

void FillPrefix(const std::string_view& str, std::vector<LengthT>& prefix_func) {
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

size_t CountSubstrings(const std::string_view& str) {
    size_t k = 0;
    std::vector<LengthT> pref;
    for (size_t i = 1; i < str.size() + 1; ++i) {
        std::string t(str.substr(0, i));
        std::reverse(t.begin(), t.end());
        FillPrefix(t, pref);
        size_t max_prefix = 0;
        for (auto p : pref) {
            max_prefix = std::max(max_prefix, p);
        }
        k += i - max_prefix;
    }
    return k;
}

}  // namespace strings

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    std::string str;
    cin >> str;
    cout << strings::CountSubstrings(str);
}