#pragma once
#include <stdio.h>
#include <string.h>
#include "CustomTypes.hpp"
#include "Board.hpp"

#include "IStrategy.hpp"
#include "SimpleStrategy.hpp"

class TreeNode {
public:
    TreeNode* parent = nullptr;
    TreeNode** child;
    int numChild = 0;
    // move from its parent
    Move move;
    
    IStrategy* strategy;
    Board board;

    // probability for this TreeNode
    // used by chance node search
    // 1.0 means it is not chance node
    double prob = 1.0;
    
    TreeNode(Board& b, Move m);
    void setStrategy(IStrategy* s);
    void genMove(char move[6]);
private:
    MoveList getOrderedMoveList();
    void generateChilds();
};