#include "SimpleStrategy.hpp"
#include "Board.hpp"
#include "TreeNode.hpp"

void SimpleStrategy::updateStrategy(TreeNode* node) {
    // no.
}

int SimpleStrategy::boardScore(Board* board) {
    return 0;
}

MoveList SimpleStrategy::getOrderedMoveList(Board* board) {
    return board->getAllMoveList();
}

Move SimpleStrategy::genMove(TreeNode* node, int leftTimeMs) {
    Board* board = &node->board;
    board->printBoard();
    MoveList list = getOrderedMoveList(board);
    int idx = Utility::getRandomNum((int)list.size());
    board->printMoveList(list);
    return list.at(idx);
}