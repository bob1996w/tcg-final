/**
 *  Early Game Strategy:
 *  used:
 *      from start of a game to one of the following happened:
 *      (1) over 10 pieces are flipped.
 *      (2) one of the following are flipped:
 *          
 *  // flip sequence: flip from 0 -> 4
 *        1 2 3 4 5 6 7 8
 *        ===============
 *  10 || 4 0 2 2 2 2 0 4
 *  20 || 1 3 3 3 3 3 3 1
 *  30 || 1 3 3 3 3 3 3 1
 *  40 || 4 0 2 2 2 2 0 4
 *  
 **/


#pragma once
#include "CustomTypes.hpp"
#include "IStrategy.hpp"

class TreeNode;
class Board;

class EarlyGameStrategy: public IStrategy {
private:
    int flipSequence[32] = {
        /* 0 */ 12, 17, 42, 47,
        /* 1 */ 21, 28, 31, 38,
        /* 2 */ 13, 14, 15, 16, 43, 44, 45, 46,
        /* 3 */ 22, 23, 24, 25, 26, 27,
                32, 33, 34, 35, 36, 37,
        /* 4 */ 11, 18, 41, 48
    };
public:
    void updateStrategy(TreeNode* node);
    int boardScore(Board* board);
    MoveList getOrderedMoveList(Board* board);
    Move genMove(TreeNode* node, int leftTimeMs);
};