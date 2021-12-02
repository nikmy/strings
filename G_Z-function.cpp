#include <iostream>
#include <string>
#include <vector>

namespace strings {

using LengthT = size_t;

void FillZ(const std::string& str, std::vector<LengthT>& z) {
    z.resize(str.length(), 0);
    z[0] = str.length();
    size_t left = 0;
    size_t right = 0;
    for (size_t i = 1; i < str.length(); ++i) {
        if (i < right) {
            z[i] = std::min(right - i, z[i - left]);
        }
        while (i + z[i] < str.length() && str[z[i]] == str[i + z[i]]) {
            z[i]++;
        }
        if (right < i + z[i]) {
            right = i + z[i];
            left = i;
        }
    }
}

}  // namespace strings

using std::cin;
using std::cout;

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    std::string str;
    std::vector<size_t> z;
    cin >> str;
    strings::FillZ(str, z);
    for (auto z_value : z) {
        cout << z_value << ' ';
    }
}