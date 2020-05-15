#include <cassert>
#include <iostream>

int main() {
    std::string word;
    uint64_t cnt;
    bool first = true;
    std::string last_word;
    uint64_t total = 0;
    while (std::getline(std::cin, word, '\t')) {
        std::cin >> cnt;
        total += cnt;
        assert(("The same words must come into the reduce job,",
            first || last_word == word));
        last_word = word;
        std::cin.get();
    }

    std::cout << last_word << "\t" << total << std::endl;

    return 0;
}

