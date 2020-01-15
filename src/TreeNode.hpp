#pragma once
#include <stdio.h>
#include <string.h>
#include "CustomTypes.hpp"
#include "Board.hpp"
#include "HashMap.hpp"
#include "NodePool.hpp"

#include "IStrategy.hpp"
#include "SimpleStrategy.hpp"
#include "EarlyGameStrategy.hpp"

#ifdef DEBUG
// debug
#include <string>
#include <vector>
#endif

class TreeNode {
public:
    TreeNode* parent = nullptr;
    TreeNode* child[80];
    int numChild = 0;
    // move from its parent
    Move move;
    HashMap* transpositionTable;
    
    IStrategy* strategy;
    Board board;

    // probability for this TreeNode
    // used by chance node search
    // 1.0 means it is not chance node
    double prob = 1.0;
    bool isChanceNode = false;
    char flippedPiece = ' ';
    bool hasBeenInChanceNode = false;

#ifdef DEBUG
    // debug: dewey decimal classification for this node.
    std::string dewey_part;
    std::string dewey() {
        std::string result = dewey_part;
        for (TreeNode* cur = parent; cur != nullptr; cur = cur->parent) {
            result.insert(0, cur->dewey_part + ".");
        }
        return result;
    }
    // TODO: put dewey on all function calling the constructor.
#endif
    // empty constuctor
    TreeNode();
    // set up for single Root TreeNode
    void setUpTreeNode(TreeNode* p, Board& b, HashMap* tb);
    // set up  for TreeNode pply a move/flip chance node to it
    void setUpTreeNode(TreeNode* p, Board& b, Move m);
    // set up  for TreeNode and a flip result
    void setUpTreeNode(TreeNode* p, Board& b, Flip f);

    void setStrategy(IStrategy* s);
    void genMove(char move[6], int timeLimitMs);
    void generateChilds(MoveList list);
private:
    MoveList getOrderedMoveList();
};