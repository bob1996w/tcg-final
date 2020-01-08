#pragma once
#include "CustomTypes.hpp"

class TreeNode; 
class Board;

class IStrategy {
public:
    virtual int boardScore(Board* board) = 0;
    virtual MoveList getOrderedMoveList(Board* board) = 0;
    virtual Move genMove(TreeNode* node) = 0;
};