#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Board.hpp"
#include "CustomTypes.hpp"

#define HASH_BITS 21
#define MAX_HASHMAP_ENTRY (1 << HASH_BITS)
#define REDUCED_HASH_MASK ((1 << HASH_BITS) - 1)

#define FLAG_EXACT 0
#define FLAG_LOWERBOUND 1
#define FLAG_UPPERBOUND 2

class HashMapNode {
public:
    Hash hash;
    int searchDepth = 0;
    int score = 0;
    int exactValueFlag = 0;
    HashMapNode* next = nullptr;
};

class HashMap {
public:
    int size;
    int entryBoardSize;
    HashMapNode** list;

    void initialize();
    bool insert(Hash fullHash, int nSearchDepth, int nScore, int nExactValueFlag);
    HashMapNode* get(Hash fullHash);
    void cleanUp();
private:
    int getKey(Hash fullHash);
};