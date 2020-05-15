#include <iostream>

int main() {
    std::string word;
    while (std::getline(std::cin, word, '\t')) {
        if (word == "\n") {
            break;
        }
        std::cout << word << "\t1" << "\n";
    }

    return 0;
}

