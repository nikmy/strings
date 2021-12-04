#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

}  // namespace strings

using std::cin;
using std::cout;

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    std::string orig;
    std::string last;
    cin >> orig >> last;

    std::vector<size_t> pref;
    strings::FillPrefix(orig + "#" + last, pref);

    size_t delta = orig.length() + 1;

    bool yes = std::any_of(pref.begin() + static_cast<int>(delta), pref.end(), [](auto x) { return x == 0; });
    if (yes) {
        cout << "Yes";
    } else {
        cout << "No\n";
        auto pos = last.size();
        std::vector<size_t> lens;
        while (pos > 0) {
            lens.push_back(pref[pos + delta - 1]);
            pos -= lens.back();
        }
        pos = 0;
        for (auto len = lens.rbegin(); len < lens.rend(); ++len) {
            cout << last.substr(pos, *len) << ' ';
            pos += *len;
        }
    }
}