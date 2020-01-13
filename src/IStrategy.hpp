#pragma once
#include "CustomTypes.hpp"

class TreeNode; 
class Board;

class IStrategy {
public:
    virtual void updateStrategy(TreeNode* node) = 0;
    virtual int boardScore(Board* board) = 0;
    virtual MoveList getOrderedMoveList(Board* board) = 0;
    virtual Move genMove(TreeNode* node, int leftTimeMs) = 0;
};