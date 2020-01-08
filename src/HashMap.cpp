#include "HashMap.hpp"

void HashMap::initialize() {
    size = MAX_HASHMAP_ENTRY;
    list = new (std::nothrow) HashMapNode*[size];
    for (int i = 0; i < size; ++i) {
        list[i] = new HashMapNode();
    }
}

// convert from full hash to reduced hash (key for hashmap.)
int getKey(Hash fullHash) {
    return fullHash & REDUCED_HASH_MASK;
}

bool HashMap::insert(Hash fullHash, int nSearchDepth, int nScore, Move nBestMove) {
    int key = getKey(fullHash);
    HashMapNode* currentList = list[key];
    HashMapNode* node = currentList;
    while (node) {
        if ((node->hash ^ fullHash) == 0) {
            // board exist, compare depth and such
        }
        node = node -> next;
    }

    // board not exist, add new node
    HashMapNode* newNode = new HashMapNode();
    newNode->hash = fullHash;
    newNode->searchDepth = nSearchDepth;
    newNode->score = nScore;
    newNode->next = currentList;
    list[key] = newNode;
    return true;
}

// return nullptr if not hit
HashMapNode* HashMap::get(Hash fullHash) {
    int key = getKey(fullHash);
    HashMapNode* currentList = list[key];
    HashMapNode* node = currentList;
    while (node) {
        if ((node->hash ^ fullHash) == 0) {
            return node;
        }
    }
    return nullptr;
}

void HashMap::cleanUp() {
    for (int i = 0; i < size; ++i) {
        HashMapNode* node = list[i];
        while (node != nullptr) {
            HashMapNode* tmp = node;
            node = node->next;
            delete tmp;
        }
    }
    delete[] list;
}