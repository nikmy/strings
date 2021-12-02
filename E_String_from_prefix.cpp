#include <iostream>
#include <string>
#include <vector>

namespace strings {

using LengthT = size_t;

std::string StringFromPrefix(const std::vector<LengthT>& pref) {
    std::string result(pref.size(), 'a');
    for (size_t i = 1; i < pref.size(); ++i) {
        if (pref[i] > 0) {
            result[i] = result[pref[i] - 1];
        } else {
            std::vector<bool> ban(26, false);
            auto k = pref[i - 1];
            while (k > 0) {
                ban[result[k] - 'a'] = true;
                k = pref[k - 1];
            }
            for (char symbol = 'b'; symbol <= 'z'; ++symbol) {
                if (!ban[symbol - 'a']) {
                    result[i] = symbol;
                    break;
                }
            }
        }
    }
    return result;
}

}  // namespace strings

using std::cin;
using std::cout;

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n = 0;
    cin >> n;
    std::vector<size_t> pref(n, 0);
    for (auto& p : pref) {
        cin >> p;
    }
    cout << strings::StringFromPrefix(pref);
}