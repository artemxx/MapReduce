#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::string word;
    uint64_t cnt;
    std::vector<std::pair<std::string, uint64_t>> words;
    while (std::cin >> word >> cnt) {
        words.emplace_back(word, cnt);
    }

    std::sort(words.begin(), words.end());
    for (const auto& it : words) {
        std::cout << it.first << " " << it.second << "\n";
    }

    return 0;
}
