#include "SimpleStrategy.hpp"
#include "Board.hpp"

int SimpleStrategy::boardScore(Board* board) {
    return 0;
}

MoveList SimpleStrategy::getOrderedMoveList(Board* board) {
    return board->getAllMoveList();
}

Move SimpleStrategy::genMove(Board* board) {
    MoveList list = getOrderedMoveList(board);
    int idx = Utility::getRandomNum((int)list.size());
    board->printMoveList(list);
    return list.at(idx);
}