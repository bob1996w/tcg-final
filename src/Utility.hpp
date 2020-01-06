#pragma once
#include <iostream>
#include <algorithm>
#include <random>

static std::mt19937_64 randomEngine(std::random_device{}());

class Utility {
public:
    static int getRandomNum(int min, int max);
    static int getRandomNum(int max);
};