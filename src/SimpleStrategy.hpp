#pragma once
#include "IStrategy.hpp"
#include "Utility.hpp"

class Board;

class SimpleStrategy : public IStrategy {
public:
    int boardScore(Board* board);
    MoveList getOrderedMoveList(Board* board);
    Move genMove(Board* board);
};