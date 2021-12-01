#include <iostream>
#include <string>
#include <vector>
#include <optional>

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

bool Contains(const std::string& str, const std::string& pattern) {
    std::vector<LengthT> prefix_func;
    FillPrefix(pattern + "#" + str, prefix_func);
    for (size_t i = 0; i < str.length(); ++i) {
        if (prefix_func[pattern.length() + i + 1] == pattern.length()) {
            return true;
        }
    }
    return false;
}

}  // namespace strings

using std::cin;
using std::cout;

using strings::Contains;

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n = 0;
    std::string initial;
    std::string rotated;

    cin >> n >> initial >> rotated;

    std::string variants[2] = {initial + "0" + initial.substr(0, n - 1), initial + "1" + initial.substr(0, n - 1)};

    int load_count = 0;
    int free_count = 0;

    for (int i = 0; i < 2; ++i) {
        if (Contains(variants[i], rotated + "1") || Contains(variants[i], "1" + rotated)) {
            ++load_count;
        }
        if (Contains(variants[i], rotated + "0") || Contains(variants[i], "0" + rotated)) {
            ++free_count;
        }
    }

    if (load_count && !free_count) {
        cout << "Yes\n";
    } else if (free_count && !load_count) {
        cout << "No\n";
    } else {
        cout << "Random\n";
    }
}