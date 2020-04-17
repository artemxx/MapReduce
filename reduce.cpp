#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::string word;
    uint64_t cnt;
    std::vector<std::pair<std::string, uint64_t>> result;
    while (std::cin >> word >> cnt) {
        if (result.empty() || result.back().first != word) {
            result.emplace_back(word, cnt);
        } else {
            result.back().second += cnt;
        }
    }

    for (const auto& it : result) {
        std::cout << it.first << " " << it.second << "\n";
    }

    return 0;
}
