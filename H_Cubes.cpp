#include <iostream>
#include <vector>
#include <algorithm>

namespace strings {

using LengthT = size_t;

template <class CharT>
void FillZ(const std::vector<CharT>& str, std::vector<LengthT>& z) {
    z.resize(str.size(), 0);
    z[0] = str.size();
    size_t left = 0;
    size_t right = 0;
    for (size_t i = 1; i < str.size(); ++i) {
        if (i < right) {
            z[i] = std::min(right - i, z[i - left]);
        }
        while (i + z[i] < str.size() && str[z[i]] == str[i + z[i]]) {
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

    size_t n = 0;
    int m = 0;
    cin >> n >> m;

    size_t merged_len = 3 * n / 2 + 1;

    std::vector<int> str(merged_len, 0);
    for (size_t i = 0; i < n; ++i) {
        cin >> str[merged_len - 1 - i];
        if (i <= n / 2) {
            str[i] = str[merged_len - 1 - i];
        }
    }
    str[n / 2] = 0;

    std::vector<size_t> z;
    strings::FillZ(str, z);

    std::vector<size_t> k;
    k.reserve(n);
    for (size_t i = n / 2; i > 0; --i) {
        if (z[merged_len - 2 * i] >= i) {
            k.push_back(n - i);
        }
    }
    k.push_back(n);

    for (auto x : k) {
        cout << x << ' ';
    }
}