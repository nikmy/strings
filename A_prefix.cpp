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

}  // namespace strings

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    std::string str;
    std::vector<size_t> pref;
    cin >> str;
    strings::FillPrefix(str, pref);
    for (auto p : pref) {
        cout << p << ' ';
    }
    cout << std::endl;
}