#include "Utility.hpp"

int Utility::getRandomNum(int min, int max) {
    std::uniform_int_distribution<int> d(min, max - 1);
    return d(randomEngine);
}

int Utility::getRandomNum(int max) {
    return getRandomNum(0, max);
}