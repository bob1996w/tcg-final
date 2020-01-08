#include "TreeNode.hpp"

TreeNode::TreeNode(Board& b, Move m) {
    board.copyBoard(b);
    strategy = new SimpleStrategy();
}

void TreeNode::setStrategy(IStrategy* s) {
    if (strategy != nullptr) {
        delete strategy;
    }
    strategy = s;
}

void TreeNode::genMove(char move[6]) {
    // called by BobAI::genMove on ROOT TreeNode.
    Move m = strategy->genMove(this);
    move[0] = m.first / 10 - 1 + 'a';
    move[1] = m.first % 10 + '0';
    move[2] = ' ';
    move[3] = m.second / 10 - 1 + 'a';
    move[4] = m.second % 10 + '0';
    move[5] = '\0';
    printf("genMove: %s\n", move);
    // return char[6] move.
}

MoveList TreeNode::getOrderedMoveList() {
    return strategy->getOrderedMoveList(&board);
}

void TreeNode::generateChilds() {
    MoveList list = getOrderedMoveList();
    // create sub-TreeNode
    // assign the sub-TreeNode's parent, move, and their board.
}