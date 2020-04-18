#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    std::string word;
    uint64_t cnt;
    std::vector<std::pair<std::string, uint64_t>> result;
    while (std::cin >> word >> cnt) {
        if (result.empty()) {
            result.emplace_back(word, cnt);
        } else {
            assert(("The same words must come into the reduce job,",
                result.back().first == word));

            result.back().second += cnt;
        }
    }

    for (const auto& it : result) {
        std::cout << it.first << " " << it.second << "\n";
    }

    return 0;
}
