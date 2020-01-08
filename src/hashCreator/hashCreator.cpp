#include <iostream>
#include <random>
#include <cmath>

using namespace std;

int main() {
    
    cout << "#pragma once\n\n" << endl;

    std::random_device rd;
    std::mt19937_64 e2(rd());
    std::uniform_int_distribution<long long int> dist(std::llround(std::pow(2,61)), std::llround(std::pow(2,62)));

    cout << endl;
    cout << "// zobrist key" << endl;
    cout << "static unsigned long long int ZOBRIST_TURN [2] = {" << dist(e2) << "LL, " << dist(e2) << "LL};" << endl;
    cout << endl;
    
    // zobrist function for each [piece][position]
    cout << "static unsigned long long int ZOBRIST_KEY [16][60] = {" << endl;
    for (int piece = 0; piece < 16; ++piece) {
        cout << "{";
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (i == 0 || i == 5 || j == 0 || j == 9) {
                    cout << "0LL, ";
                }
                else {
                    cout << dist(e2) << ", ";
                }
            }
        }
        cout << "}," << endl;
    }
    cout << "};" << endl;
}