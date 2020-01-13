#include "SecondStrategy.hpp"
#include "TreeNode.hpp"
#include "Board.hpp"

//int SecondStrategy::plyBasicScore[] = {0, 100, 65, 30, 16, 8, 4, 1, 0, 100, 65, 30, 16, 8, 4, 1};
int SecondStrategy::plyBasicScore[] = {0, 165, 80, 50, 40, 30, 75, 20, /**/ 0, 165, 80, 50, 40, 30, 75, 20};

void SecondStrategy::updateStrategy(TreeNode* node) {
    // no update
}

/*
int SecondStrategy::oldBoardScore(Board* board) {
    // TODO: cache from the Transposition table
    int ourTurn = rootColor;
    int theirTurn = !rootColor;
    int score = 0;
    int ourPType, theirPType;
    int temp;
    
    for (int i = 0; i < board->numPiece[ourTurn]; ++i) {
        ourPType = board->piece[ourTurn][i]->pieceType;
        if ((ourPType & 7) == PT_RC) {
            // is Cannon:
            // add plyBasicScore[Cannon] * (4 * number of our pieces left + 1 * number of enemy's pieces left)
            score += plyBasicScore[PT_RC] * (4 * board->numPiece[ourTurn] + board->numPiece[theirTurn]);
        }
        else {
            // for each of our piece x that can eat enemy's piece y,
            // we add plyBasicScore[x->pieceType] to total score.
            temp = 0;
            for (int j = 0; j < board->numPiece[theirTurn]; ++j) {
                // if we can eat that
                theirPType = board->piece[theirTurn][j]->pieceType;
                if (CAN_EAT_BY_MOVE[ourPType][theirPType]) {
                    temp += 1;
                }
            }
            score += plyBasicScore[ourPType & 7] * 4 * temp;
        }
    }
    return score;
}
*/

/*
int SecondStrategy::oldBoardScore2(Board* board) {
    int ourTurn = rootColor, theirTurn = !rootColor;
    int ourScore = 0, theirScore = 0;
    int ourPType, theirPType;
    for (int i = 0; i < board->numPiece[ourTurn]; ++i) {
        ourPType = board->piece[ourTurn][i]->pieceType;
        for (int j = 0; j < board->numPiece[theirTurn]; ++j) {
            theirPType = board->piece[theirTurn][j]->pieceType;
            if (CAN_EAT_BY_MOVE[ourPType][theirPType]) {
                ourScore += plyBasicScore[theirPType];
            }
            if ((ourPType & 7) == PT_RC) {
                // cannon can jump-eat all enemy pieces
                ourScore += plyBasicScore[theirPType];
            }
            if (CAN_EAT_BY_MOVE[theirPType][ourPType]) {
                theirScore += plyBasicScore[ourPType];
            }
            if ((theirPType & 7) == PT_RC) {
                // cannon can jump-eat all enemy pieces
                theirScore += plyBasicScore[ourPType];
            }
        }
    }
    return ourScore - theirScore;
}
*/

// calculate current ways to eat the pieces
int SecondStrategy::boardScore(Board* board) {
    int ourTurn = rootColor, theirTurn = !rootColor;
    int ourScore = 0, theirScore = 0;
    int ourPType, theirPType;
    MoveList l = getMoveListEat(board, ourTurn);
    for (int i = 0; i < l.size(); ++i) {
        theirPType = board->block[l.at(i).second]->pieceType;
        ourScore += plyBasicScore[theirPType];
    }
    l = getMoveListEat(board, theirTurn);
    for (int i = 0; i < l.size(); ++i) {
        ourPType = board->block[l.at(i).second]->pieceType;
        theirScore += plyBasicScore[ourPType];
    }
    return ourScore - theirScore;
}

MoveList SecondStrategy::getOrderedMoveList(Board* board) {
    MoveList l1 = getMoveListEat(board, board->turn);
    MoveList l2 = getMoveListMove(board);
    //Move flip = getMoveListFlip(board);
    l1.reserve(l1.size() + l2.size() + 1);
    l1.insert(l1.end(), l2.begin(), l2.end());
    //l1.emplace_back(flip);
    //fprintf(stdout, "getOrderedMoveList::list size = %d\n", l1.size());
    //fflush(stdout);
    return l1;
}

Move SecondStrategy::genMove(TreeNode* node, int leftTimeMs) {
    transpositionTable = node->transpositionTable;
    searchExceedTimeLimit = false;
    rootColor = node->board.turn;
#ifdef DEBUG
    fprintf(stdout, "::%s::genMove, turn=%d::node->numChild = %d\n", node->dewey().c_str(), node->board.turn, node->numChild);
    fflush(stdout);
#endif
    if (node->numChild == 0) {
        fprintf(stdout, "genMove::generating child\n");
        fflush(stdout);
        MoveList moves = getOrderedMoveList(&node->board);
        fprintf(stdout, "genMove::generated %d childs\n", moves.size()); fflush(stdout);
        if (moves.size() == 0) {
            fprintf(stdout, "return genMoveListFlip\n"); fflush(stdout);
            return getMoveListFlip(&node->board);
        }
        node->generateChilds(moves);
    }

    // temporary time limit
    int timeLimitMs = (node->board.depth < 10)? 1000 : 
            (node->board.depth < 80 && leftTimeMs >= 300000) ? (leftTimeMs - 20000) / (120 - node->board.depth) :
            (leftTimeMs - 20000) / 40;
    int perChildTimeLimitMs = timeLimitMs / node->numChild;
    fprintf(stdout, "genMove::depth %d, time: T = %d, t = %d, c = %d ms\n", node->board.depth, leftTimeMs, timeLimitMs, perChildTimeLimitMs);
    fflush(stdout);

    int bestScore = INT_MIN;
    int bestChild = -1;
    for (int i = 0; i < node->numChild; ++i) {
        int score = iterativeDeepening(node->child[i], perChildTimeLimitMs);
        fprintf(stdout, "genMove::ID %d / %d", i + 1, node->numChild);
        node->child[i]->board.printMove(node->child[i]->move);
        fprintf(stdout, ", %d\n", score);
        fflush(stdout);
        if (score > bestScore) {
            fprintf(stdout, "genMove::score %d > bestScore %d\n", score, bestScore);
            bestScore = score;
            bestChild = i;
        }

        // TODO: board needs to check win/lose conditions
    }
    fprintf(stdout, "genMove::bestScore = %d\n", bestScore);
    // TODO: fix the scoring to use the flip
    if (bestScore < 0 && node->board.numPieceUnflipped[rootColor] > 0) {
        fprintf(stdout, "genMove::use flip, numPieceUnflipped = red[%d], black[%d]\n", node->board.numPieceUnflipped[TURN_RED], node->board.numPieceUnflipped[TURN_BLACK]);
        return getMoveListFlip(&node->board);
    }
    else {
        return node->child[bestChild]->move;
    }

}

int SecondStrategy::iterativeDeepening(TreeNode* node, int timeLimitMs) {
    // set timer

    auto d = std::chrono::steady_clock::now().time_since_epoch();
    int startTimeMs = (int)std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
    int endTimeMs = startTimeMs + timeLimitMs;
    
    int maxDepth = 1;
    int bestScore = 0;
    while (true) {
        fprintf(stdout, "ID depth %d\n", maxDepth);
        fflush(stdout);
        int currentTimeMs = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        if (currentTimeMs >= endTimeMs) {
            break;
        }
        int currentScore = search(node, INT_MIN, INT_MAX, maxDepth, currentTimeMs, endTimeMs);

        if (!searchExceedTimeLimit) {
            bestScore = currentScore;
        }
        if (searchExceedTimeLimit) {
            break;
        }
        // to prevent searching too much
        if (maxDepth > 10) {
            break;
        }
        ++maxDepth;
    }
    return bestScore;
}

int SecondStrategy::search(TreeNode* node, int alpha, int beta, int depth, int startTimeMs, int timeLimitMs, bool byFlip/* = false */) {
    // 展開child
    if (node == nullptr) {
        printf("search::node is nullptr\n");
        fflush(stdout);
    }
    
    // if this board is a win for rootColor
    if (node->board.winner == rootColor) {
        return 100000;
    }

    // if this board is a lose for rootColor
    if (node->board.winner == !rootColor) {
        return -100000;
    }
    
    // check if current position in transposition table
    HashMapNode* tableResult = transpositionTable->get(node->board.hash);
    if (tableResult != nullptr && tableResult->searchDepth >= depth && tableResult->isExactValue) {
        printf("early return\n");
        return tableResult->score;
    }

    // generate possible moves and child
    if (node->numChild == 0) {
        MoveList moves = getOrderedMoveList(&node->board);
        if (moves.size() == 0) {
#ifdef DEBUG
            //fprintf(stdout, "%s::search::depth %d::boardDepth %d::no move list, return boardScore\n", node->dewey().c_str(), depth, node->board.depth);
#endif
            return boardScore(&node->board);
        }
        node->generateChilds(moves);
    }
#ifdef DEBUG
    //fprintf(stdout, "%s::search::depth %d::boardDepth %d::before unfold childs; numChild = %d\n", node->dewey().c_str(),depth, node->board.depth, node->numChild);fflush(stdout);
#endif

    // check timeout
    int currentTimeMs = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    int elapsedTimeMs = currentTimeMs - startTimeMs;
    
    if (elapsedTimeMs - startTimeMs >= timeLimitMs) {
        fprintf(stdout, "search::searchExceedTimeLimit\n");fflush(stdout);
        searchExceedTimeLimit = true;
    }

    // if is a terminal node || over depth -> abort
    if (searchExceedTimeLimit || node->numChild == 0 || depth <= 0 /* || win/lose */) {
        return boardScore(&node->board);
    }

    // if this is a chance node
    if (node->isChanceNode) {
#ifdef DEBUG
        //fprintf(stdout, "%s::search::chanceNode\n", node->dewey().c_str());
#endif
        // algorithm Star0_F3.0'
        // the depths of a chance node's child are deemed same,
        // so we don't minus 1 on depth.

        // the text book says all possible pieces has equal chances, so...
        int chanceNodeScore = 0;
        int totalPossiblePieces = 0;
        for (int i = 0; i < node->numChild; ++i) {
                ++totalPossiblePieces;
#ifdef DEBUG
                fprintf(stdout, "%s::search::get Chance node score for child %d", node->dewey().c_str(), i);
                fflush(stdout);
#endif
                chanceNodeScore += search(node->child[i], alpha, beta, depth, startTimeMs, timeLimitMs);
            
        }
        return chanceNodeScore / totalPossiblePieces;
    }
    
    if (node->board.turn == rootColor) {
#ifdef DEBUG
        //fprintf(stdout, "%s::search::MAX node, %d\n", node->dewey().c_str(), node->board.turn);
        //fflush(stdout);
#endif
        // max node, algorithm F4'
        int m = INT_MIN, t; // m is current best lower bound, fail soft
        if (tableResult != nullptr) { m = tableResult->score; }
        m = MAX(m, search(node->child[0], alpha, beta, depth - 1, startTimeMs, timeLimitMs)); // test first branch, enter G4'
        if (m >= beta) {
            // beta cut-off
            transpositionTable->insert(node->board.hash, depth, m, false);
            return m;
        }
        for (int i = 1; i < node->numChild; ++i) {
            t = search(node->child[i], m, m + 1, depth - 1, startTimeMs, timeLimitMs); // null-window search
            if (t > m) {
                // failed high
                if (depth < 3 || t >= beta) {
                    m = t;
                }
                else {
                    // beta cut-off, re-search
                    m = search(node->child[i], t, beta, depth - 1, startTimeMs, timeLimitMs); // G4'
                }
            }
            if (m >= beta) {
                // beta cut-off
                transpositionTable->insert(node->board.hash, depth, m, false);
                return m;
            }
        }
        if (m > beta) {
            transpositionTable->insert(node->board.hash, depth, m, true);
        }
        else {
            transpositionTable->insert(node->board.hash, depth, m, false);
        }
        return m;
    }
    else {
#ifdef DEBUG
        //fprintf(stdout, "%s::search::MIN node, %d\n", node->dewey().c_str(), node->board.turn);
        //fflush(stdout);
#endif
        // min node, algorithm G4'
        int m = INT_MAX, t; // m is current best upper bound, fail soft
        m = MIN(m, search(node->child[0], alpha, beta, depth - 1, startTimeMs, timeLimitMs));
        if (m <= alpha) {
            // alpha cut-off
            transpositionTable->insert(node->board.hash, depth, m, false);
            return m;
        }
        for (int i = 1; i < node->numChild; ++i) {
            t = search(node->child[i], m - 1, m, depth - 1, startTimeMs, timeLimitMs); // null-window search
            if (t < m) {
                // failed low
                if (depth < 3 || t <= alpha) {
                    m = t;
                }
                else {
                    // alpha cut-off, re-search
                    m = search(node->child[i], alpha, t, depth - 1, startTimeMs, timeLimitMs);
                }
            }
            if (m <= alpha) {
                // alpha cut-off
                transpositionTable->insert(node->board.hash, depth, m, false);
                return m;
            }
        }
        if (m < alpha) {
            transpositionTable->insert(node->board.hash, depth, m, true);
        }
        else {
            transpositionTable->insert(node->board.hash, depth, m, false);
        }
        return m;
    }
}

Move SecondStrategy::getMoveListFlip(Board* board) {
    if (board->numPieceUnflipped[board->turn] == 0) {
        // no more piece to flip!
        return Move(-1, -1);
    }
    // TODO: generate MoveList containing only one flip move that is the best
    int bestPos = -1, pos = 0;
    int bestPosScore = 0, posScore = 0;
    for (int fidx = 0; fidx < 32; ++fidx) {
        pos = flipSequence[fidx];
        if (board->block[pos] == nullptr) {
            // is a dark piece
            bestPos = pos;
            break;
        }
    }
    return Move(pos, pos);
}

MoveList SecondStrategy::getMoveListEat(Board* board, int turn) {
    MoveList list;
    // int turn = board->turn;
    for (int pidx = 0; pidx < board->numPiece[turn]; ++pidx) {
        Piece* pc = board->piece[turn][pidx];
        int src = pc->pos;
        int pType = pc->pieceType;
        for (int didx = 0; didx < MOVE_NUM[src]; ++didx) {
            int dst = src + MOVE_DIR[src][didx];
            if (board->block[dst]!=nullptr && !(board->block[dst]->isDead) && CAN_EAT_BY_MOVE[pType][board->block[dst]->pieceType]) {
                list.emplace_back(src, dst);
            }
        }
        if ((pType & 7) == 6) {
            for (int jidx = 0; jidx < JUMP_NUM[src]; ++jidx) {
                bool existJumpedPiece = false;
                for (int dst = src + JUMP_DIR[src][jidx]; !IS_OUT[dst]; dst += JUMP_DIR[src][jidx]) {
                    if (board->block[dst] == nullptr || !board->block[dst]->isDead) {
                        if (!existJumpedPiece){existJumpedPiece = true;}
                        else if (board->block[dst] != nullptr && CAN_EAT_BY_JUMP[pType][board->block[dst]->pieceType]) {
                            list.emplace_back(src, dst);
                            break;
                        }
                        else {
                            break;
                        }
                    }
                }
            }
        }
    }
    return list;
}

MoveList SecondStrategy:: getMoveListMove(Board* board) {
    MoveList list;
    int turn = board->turn;
    for (int pidx = 0; pidx < board->numPiece[turn]; ++pidx) {
        Piece* pc = board->piece[turn][pidx];
        int src = pc->pos;
        int pType = pc->pieceType;
        for (int didx = 0; didx < MOVE_NUM[src]; ++didx) {
            int dst = src + MOVE_DIR[src][didx];
            if (board->block[dst]!=nullptr && board->block[dst]->isDead) {
                list.emplace_back(src, dst);
            }
        }
    }
    return list;
}