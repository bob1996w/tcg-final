#pragma once
#include "CustomTypes.hpp"
#include "IStrategy.hpp"
#include "HashMap.hpp"
#include <climits>
#include <chrono>

#define MAX(a,b) ((a > b) ? a : b)
#define MIN(a,b) ((a > b) ? b : a)

class TreeNode;
class Board;



class SecondStrategy: public IStrategy {
public:
    void updateStrategy(TreeNode* node);
    int boardScore(Board* board);
    MoveList getOrderedMoveList(Board* board);
    Move genMove(TreeNode* node, int leftTimeMs);

private:
    int rootColor = 2; // TURN_INITIAL
    bool searchExceedTimeLimit = false;
    static const int strategyTag = 2;
    static int plyBasicScore[16];
    HashMap* transpositionTable;

/* // flip sequence: flip from 0 -> 4
 *        1 2 3 4 5 6 7 8
 *        ===============
 *  10 || 4 0 2 2 2 2 0 4
 *  20 || 1 3 3 3 3 3 3 1
 *  30 || 1 3 3 3 3 3 3 1
 *  40 || 4 0 2 2 2 2 0 4
 */
    int flipSequence[32] = {
        /* 0 */ 12, 17, 42, 47,
        /* 1 */ 21, 28, 31, 38,
        /* 2 */ 13, 14, 15, 16, 43, 44, 45, 46,
        /* 3 */ 22, 23, 24, 25, 26, 27,
                32, 33, 34, 35, 36, 37,
        /* 4 */ 11, 18, 41, 48
    };
    // generate one best position to flip
    Move getMoveListFlip(Board* board, int turn);
    // move list that only contains eat
    MoveList getMoveListEat(Board* board, int turn);
    // move list that only contains move
    MoveList getMoveListMove(Board* board);

    int iterativeDeepening(TreeNode* node, int timeLimitMs);
    // MiniMax NegaScout, algorithm F4' and G4'.
    int search(TreeNode* node, int alpha, int beta, int maxDepth, int startTimeMs, int timeLimitMs);
    // int searchNegaScout(TreeNode* node, int depth, int alpha, int beta, int startTimeMs, int timeLimitMs);
};