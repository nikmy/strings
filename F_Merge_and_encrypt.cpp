#include <iostream>
#include <string>
#include <vector>

namespace strings {

using LengthT = size_t;

void FillPrefix(const std::string& str, std::vector<LengthT>& prefix_func) {
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

}  // namespace strings

using std::cin;
using std::cout;

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n = 0;
    cin >> n;

    std::string result;
    cin >> result;
    std::vector<size_t> pref;
    std::string str;
    pref.reserve(1000000);
    for (size_t i = 1; i < n; ++i) {
        cin >> str;
        std::string merged = str + "#" + result.substr(result.length() - std::min(result.size(), str.size()));
        pref.resize(merged.length());
        strings::FillPrefix(merged, pref);
        result += (str.c_str() + pref.back());
    }
    cout << result;
}