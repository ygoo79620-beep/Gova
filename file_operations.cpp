#include <iostream>
#include <cstdio>
#include <vector>
#include <Windows.h>

std::string generateRandomName(const std::vector<std::string>& possibleNames) {
    std::srand(static_cast<unsigned>(std::time(0)));
    int randomIndex = std::rand() % possibleNames.size();
    return possibleNames[randomIndex];
}
