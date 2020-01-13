#include "TreeNode.hpp"

TreeNode::TreeNode(TreeNode* p, Board& b, HashMap* tb) {
    this->parent = p;
    if (p != nullptr && p->hasBeenInChanceNode) {
        hasBeenInChanceNode = true;
    }
    board.copyBoard(b);
    strategy = new EarlyGameStrategy();
    transpositionTable = tb;
}

TreeNode::TreeNode(TreeNode* p, Board& b, Move m) {
    this->parent = p;
    if (p != nullptr && p->hasBeenInChanceNode) {
        hasBeenInChanceNode = true;
    }
    board.copyBoard(b);
    strategy = new EarlyGameStrategy();
    // TODO: revive chance node?
    // NOTE: the chance node is on halt. Too complicated.
    if (m.first == m.second) {
        // is a flip: chance node
        // generate all childs
        /*
        isChanceNode = true;
        numChild = 0;
        child = new TreeNode*[14]; // maximum 14
        for (int i = 0; i < 14; ++i) {
            int pType = ALL_PIECE_TYPE[i];
            int color = pType >> 3, type = pType & 7;
            if (allPieceTypeNum[type] - board.flippedNumPiece[color][type] > 0) {
                Flip f = Flip(m.first, PIECE2CHAR[pType]);
                child[numChild] = new TreeNode(this, b, f);
#ifdef DEBUG
                child[numChild]->dewey_part = "c" + std::to_string(numChild);
#endif
                ++numChild;
            }
        }
        */
        //fprintf(stderr, "TreeNode:: finish creating chance node, numChild = %d\n", numChild);
    }
    else {
        // just apply a move to it
        move = m;
        board.applyMove(m);
    }
}

TreeNode::TreeNode(TreeNode* p, Board& b, Flip f) {
    this->parent = p;
    // this->move is not copied from Flip.
    if (p != nullptr && p->hasBeenInChanceNode) {
        hasBeenInChanceNode = true;
    }
    board.copyBoard(b);
    strategy = new EarlyGameStrategy();
    flippedPiece = f.second;
    board.applyFlip(f);
}

TreeNode::~TreeNode() {
    // FIXME: something wrong when deleting the TreeNode.
    if (strategy != nullptr) {
        delete strategy;
    }
    if (child != nullptr) {
        for (int i = 0; i < numChild; ++i) {
            if (child[i] != nullptr) {
                delete child[i];
            }
        }
        delete[] child;
    }
}

void TreeNode::setStrategy(IStrategy* s) {
    if (strategy != nullptr) {
        delete strategy;
    }
    strategy = s;
}

void TreeNode::genMove(char move[6], int timeLimitMs) {
    // called by BobAI::genMove on ROOT TreeNode.
    strategy->updateStrategy(this);
    Move m = strategy->genMove(this, timeLimitMs);
    printf("genMove: Move(%d, %d)\n", m.first, m.second);
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

void TreeNode::generateChilds(MoveList list) {
    numChild = (int) list.size();
    child = new TreeNode*[numChild];
    // printf(" TreeNode::generateChilds, this.board.turn = %d\n", board.turn);

    for (int i = 0; i < numChild; ++i) {
        Move m = list[i];
        child[i] = new TreeNode(this, board, m);
#ifdef DEBUG
        child[i]->dewey_part = std::to_string(i);
#endif
        // printf(" TreeNode::child generated, child[%d].board.turn = %d\n", i, child[i]->board.turn);
    }
    // fflush(stdout);
}