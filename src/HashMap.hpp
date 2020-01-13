#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Board.hpp"
#include "CustomTypes.hpp"

#define HASH_BITS 21
#define MAX_HASHMAP_ENTRY (1 << HASH_BITS)
#define REDUCED_HASH_MASK ((1 << HASH_BITS) - 1)

class HashMapNode {
public:
    Hash hash;
    int searchDepth = 0;
    int score = 0;
    bool isExactValue = false;
    HashMapNode* next = nullptr;
};

class HashMap {
public:
    int size;
    int entryBoardSize;
    HashMapNode** list;

    void initialize();
    bool insert(Hash fullHash, int nSearchDepth, int nScore, bool nIsExactValue);
    HashMapNode* get(Hash fullHash);
    void cleanUp();
private:
    int getKey(Hash fullHash);
};