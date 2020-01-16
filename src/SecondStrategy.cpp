#include "SecondStrategy.hpp"
#include "TreeNode.hpp"
#include "Board.hpp"

//int SecondStrategy::plyBasicScore[] = {0, 100, 65, 30, 16, 8, 4, 1, 0, 100, 65, 30, 16, 8, 4, 1};
int SecondStrategy::plyBasicScore[] = {0, 165, 80, 50, 40, 30, 75, 20, /**/ 0, 165, 80, 50, 40, 30, 75, 20};

void SecondStrategy::updateStrategy(TreeNode* node) {
    // not updated from here
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
/*
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
    printf("%d\n", ourScore - theirScore);
    return ourScore - theirScore;
}
*/

/*
int SecondStrategy::boardScore(Board* board) {
    int ourTurn = rootColor, theirTurn = !rootColor;
    int ourScore = 0, theirScore = 0;
    int ourPType, theirPType;

    // eaten pieces
    for (int type = 1; type < 8; ++type) {
        ourScore += plyBasicScore[type] * (board->flippedNumPiece[theirTurn][type] - board->useableNumPiece[theirTurn][type]);
        theirScore += plyBasicScore[type] * (board->flippedNumPiece[ourTurn][type] - board->useableNumPiece[ourTurn][type]);
    }

    // current on-board pieces
    for (int i = 0; i < board->numPiece[ourTurn]; ++i) {
        ourPType = board->piece[ourTurn][i]->pieceType;
        for (int j = 0; j < board->numPiece[theirTurn]; ++j) {
            theirPType = board->piece[theirTurn][j]->pieceType;
            if (CAN_EAT_BY_MOVE[ourPType][theirPType]) {
                ourScore += plyBasicScore[theirPType] / DIST[board->piece[ourTurn][i]->pos][board->piece[theirTurn][j]->pos];
            }
            if ((ourPType & 7) == PT_RC) {
                // cannon can jump-eat all enemy pieces
                ourScore += plyBasicScore[theirPType] / DIST[board->piece[ourTurn][i]->pos][board->piece[theirTurn][j]->pos];
            }
            
            if (CAN_EAT_BY_MOVE[theirPType][ourPType]) {
                theirScore += plyBasicScore[ourPType] / DIST[board->piece[ourTurn][i]->pos][board->piece[theirTurn][j]->pos];
            }
            if ((theirPType & 7) == PT_RC) {
                // cannon can jump-eat all enemy pieces
                theirScore += plyBasicScore[ourPType] / DIST[board->piece[ourTurn][i]->pos][board->piece[theirTurn][j]->pos];
            }
        }
    }

    // piece position score
    for (int i = 0; i < board->numPiece[ourTurn]; ++i) {
        ourScore += BOARD_POSITION_SCORE[board->piece[ourTurn][i]->pos];
    }
    
    for (int i = 0; i < board->numPiece[theirTurn]; ++i) {
        theirScore += BOARD_POSITION_SCORE[board->piece[theirTurn][i]->pos];
    }
    
    //printf("%d\n", ourScore - theirScore);
    return ourScore - theirScore;
    //return ourScore;
}
*/

int SecondStrategy::boardScore(Board* board) {
    int ourTurn = rootColor, theirTurn = !rootColor;
    int ourScore = 0, theirScore = 0;
    int ourPType, theirPType;

    int ediblePly[2][8] = {0}; // how many ply can we eat?
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            if (CAN_EAT_BY_MOVE[i][j]) {
                ediblePly[i >> 3][i & 7] += board->useableNumPiece[j >> 3][j & 7];
            }
            if (CAN_EAT_BY_JUMP[i][j]) {
                ediblePly[i >> 3][i & 7] += board->useableNumPiece[j >> 3][j & 7];
            }
        }
    }

    for (int i = 0; i < board->numPiece[ourTurn]; ++i) {
        ourPType = board->piece[ourTurn][i]->pieceType;
        if ((ourPType & 7) == PT_RP) {
            ourScore += plyBasicScore[ourPType] * board->useableNumPiece[theirTurn][PT_RK] + ediblePly[ourTurn][ourPType & 7] + BOARD_POSITION_SCORE[board->piece[ourTurn][i]->pos];
        }
        else {
            ourScore += plyBasicScore[ourPType] + ediblePly[ourTurn][ourPType & 7] + BOARD_POSITION_SCORE[board->piece[ourTurn][i]->pos];
        }
    }

    for (int i = 0; i < board->numPiece[theirTurn]; ++i) {
        theirPType = board->piece[theirTurn][i]->pieceType;
        if ((theirPType& 7) == PT_RP) {
            theirScore += plyBasicScore[ourPType] * board->useableNumPiece[theirTurn][PT_RK] + ediblePly[theirTurn][theirPType& 7] + BOARD_POSITION_SCORE[board->piece[theirTurn][i]->pos];
        }
        else {
            theirScore += plyBasicScore[ourPType] + ediblePly[theirTurn][theirPType& 7] + BOARD_POSITION_SCORE[board->piece[theirTurn][i]->pos];
        }
    }

    ourScore += (board->numPiece[ourTurn]) << 7;
    theirScore += (board->numPiece[theirTurn]) << 7;
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

    // generate child
    if (node->numChild == 0) {
        fflush(stdout);
        MoveList moves = getOrderedMoveList(&node->board);
        fprintf(stdout, "genMove::generated %d childs\n", moves.size()); fflush(stdout);
        if (moves.size() == 0) {
            fprintf(stdout, "return genMoveListFlip\n"); fflush(stdout);
            return getMoveListFlip(&node->board, rootColor);
        }
        node->generateChilds(moves);
    }

    // temporary time limit
    int timeLimitMs = (node->board.depth < 10)? 1000 : 
            (node->board.depth < 80 && leftTimeMs >= 300000) ? (leftTimeMs - 300000) / (120 - node->board.depth) :
            (leftTimeMs - 20000) / 40;
    fprintf(stdout, "genMove::depth %d, time: T = %d, t = %d\n", node->board.depth, leftTimeMs, timeLimitMs);
    fflush(stdout);

    searchCounter = 0;
    // iterative deepening
    std::pair<int, int> bestCandidate = iterativeDeepening(node, timeLimitMs);
    int bestChild = bestCandidate.first;
    int bestScore = bestCandidate.second;

    int currentBoardScore = boardScore(&node->board);
    fprintf(stdout, "genMove::currentBoardScore = %d, bestScore = %d\n", currentBoardScore, bestScore);
    
    // decide final move:
    //  if the best move is "eat", do so;
    //  if the best move is "move":
    //      if the score of best move is worse than current board's score && we have unflipped, flip;
    //      else, send that best move.
    if (!node->board.block[node->child[bestChild]->move.second]->isDead) {
        // is eat
        return node->child[bestChild]->move;
    }
    else if (bestScore <= currentBoardScore && node->board.numPieceUnflipped[rootColor] > 0) {
        //fprintf(stdout, "genMove::use flip, numPieceUnflipped = red[%d], black[%d]\n", node->board.numPieceUnflipped[TURN_RED], node->board.numPieceUnflipped[TURN_BLACK]);
        return getMoveListFlip(&node->board, rootColor);
    }
    else {
        return node->child[bestChild]->move;
    }

}

// return std::pair<child#, childScore>
std::pair<int, int> SecondStrategy::iterativeDeepening(TreeNode* node, int timeLimitMs) {
    int startTimeMs = Utility::timer();
    int endTimeMs = startTimeMs + timeLimitMs;

    int depth = 3;
    int bestScoreForFinishedDepth = INT_MIN, bestChildForFinishedDepth = -1;
    int bestScoreForCurrentDepth = INT_MIN, bestChildForCurrentDepth = -1;
    int score;
    searchExceedTimeLimit = false;

    while (true) {
        //fprintf(stdout, "ID::search depth %d\n", depth);
        for (int c = 0; c < node->numChild; ++c) {
            // score = search(node->child[c], INT_MIN, INT_MAX, depth, endTimeMs);
            // negaScout searched from the child, so we get a minus score.
            score = (-1) * searchNegaScout(node->child[c], depth, INT_MIN, INT_MAX, endTimeMs);
            /*
            fprintf(stdout, "ID::depth %2d, child %2d, move %d %c -> %d %c, score %5d\n",
                    depth, c, node->child[c]->move.first, PIECE2CHAR[node->board.block[node->child[c]->move.first]->pieceType],
                    node->child[c]->move.second, 
                    (node->board.block[node->child[c]->move.second]->isDead ? '_' : PIECE2CHAR[node->board.block[node->child[c]->move.second]->pieceType]), 
                    score);
            fflush(stdout);
            */
            if (searchExceedTimeLimit) {
                // return bestScoreForFinishedDepth and bestChildForFinishedDepth
                break;
            }
            else {
                if (score > bestScoreForCurrentDepth) {
                    bestScoreForCurrentDepth = score;
                    bestChildForCurrentDepth = c;
                }
            }
        }
        if (searchExceedTimeLimit) {
            break;
        }
        else {
            bestScoreForFinishedDepth = bestScoreForCurrentDepth;
            bestChildForFinishedDepth = bestChildForCurrentDepth;
            bestScoreForCurrentDepth = INT_MIN;
            bestChildForCurrentDepth = -1;
            ++depth;
        }
        if (depth > 30) {
            break;
        }
    }
    return std::pair<int, int>(bestChildForFinishedDepth, bestScoreForFinishedDepth);
}


int SecondStrategy::searchNegaScout(TreeNode* node, int depth, int alpha, int beta, int endTimeMs) {
    int originalAlpha = alpha;
    int currentLowerBound = INT_MIN;
    int currentUpperBound = beta;

    HashMapNode* tableResult = transpositionTable->get(node->board.hash);
    if (tableResult != nullptr) {
        if (tableResult->searchDepth >= depth) {
            if (tableResult->exactValueFlag == FLAG_EXACT) {
                return tableResult->score;
            }
            else if (tableResult->exactValueFlag == FLAG_LOWERBOUND) {
                alpha = std::max(alpha, tableResult->score);
            }
            else if (tableResult->exactValueFlag == FLAG_UPPERBOUND) {
                beta = std::min(beta, tableResult->score);
            }
            if (alpha >= beta) {
                // cut off
                return tableResult->score;
            }
        }
        else {
            if (tableResult->exactValueFlag == FLAG_EXACT) {
                currentUpperBound = tableResult->score;
            }
        }
    }

    int currentTimeMs = Utility::timer();

    if (node->board.winner == node->board.turn) {
        return 100000;
    }

    if (node->board.winner == !node->board.turn) {
        return -100000;
    }

    if (currentTimeMs >= endTimeMs) {
        searchExceedTimeLimit = true;
    }

    if (depth == 0 || searchExceedTimeLimit) {
        return boardScore(&node->board) * (node->board.turn == rootColor? 1 : -1);
    }

    // generate child
    if (node->numChild == 0) {
        MoveList moves = getOrderedMoveList(&node->board);
        if (moves.size() == 0) {
            return boardScore(&node->board) * (node->board.turn == rootColor? 1 : -1);
        }
        node->generateChilds(moves);
    }

    int temp;
    for (int i = 0; i < node->numChild; ++i) {
        temp = (-1) * searchNegaScout(node->child[i], depth - 1, (-1) * currentUpperBound, (-1) * std::max(alpha, currentLowerBound), endTimeMs);
        if (temp > currentLowerBound) {
            if (currentUpperBound == beta || depth < 3 || temp >= beta) {
                currentLowerBound = temp;
            }
            else {
                // re-search
                currentLowerBound = (-1) * searchNegaScout(node->child[i], depth - 1, (-1) * beta, (-1) * temp, endTimeMs);
            }
        }
        
        if (currentLowerBound >= beta) {
            // beta cut-off
            // save to hashTable?
            // transpositionTable->insert(node->board.hash, depth, currentLowerBound, FLAG_LOWERBOUND);
            break;
        }
        // set up a null window
        currentUpperBound = std::max(alpha, currentLowerBound) + 1; 
    }
    if (currentLowerBound <= originalAlpha && !searchExceedTimeLimit) {
        transpositionTable->insert(node->board.hash, depth, currentLowerBound, FLAG_UPPERBOUND);
    }
    else if (currentLowerBound >= beta && !searchExceedTimeLimit) {
        transpositionTable->insert(node->board.hash, depth, currentLowerBound, FLAG_LOWERBOUND);
    }
    else if (!searchExceedTimeLimit){
        transpositionTable->insert(node->board.hash, depth, currentLowerBound, FLAG_EXACT);
    }
    return currentLowerBound;
}


int SecondStrategy::search(TreeNode* node, int alpha, int beta, int depth, int endTimeMs) {
    //printf("search::%d %d\n", node->board.turn, rootColor);
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
    if (tableResult != nullptr && tableResult->searchDepth >= depth && tableResult->exactValueFlag == FLAG_EXACT) {
        return tableResult->score;
    }

    // generate possible moves and child
    if (node->numChild == 0) {
        MoveList moves = getOrderedMoveList(&node->board);
        if (moves.size() == 0) {
            return boardScore(&node->board);
        }
        node->generateChilds(moves);
    }

    // check timeout
    int currentTimeMs = Utility::timer();
    
    if (currentTimeMs >= endTimeMs) {
        searchExceedTimeLimit = true;
    }

    // if is a terminal node || over depth -> abort
    if (searchExceedTimeLimit || node->numChild == 0 || depth <= 0) {
        return boardScore(&node->board);
    }
    
    if (node->board.turn == rootColor) {
        // max node, algorithm F4'
        int m = INT_MIN, t; // m is current best lower bound; fail soft
        if (tableResult != nullptr) { m = tableResult->score; }
        m = std::max(m, search(node->child[0], alpha, beta, depth - 1, endTimeMs)); // test first branch, enter G4'
        if (m >= beta) {
            // beta cut-off
            transpositionTable->insert(node->board.hash, depth, m, FLAG_LOWERBOUND);
            return m;
        }
        for (int i = 1; i < node->numChild; ++i) {
            t = search(node->child[i], m, m + 1, depth - 1, endTimeMs); // null-window search
            if (t > m) {
                // failed high
                if (depth < 3 || t >= beta) {
                    m = t;
                }
                else {
                    // beta cut-off, re-search
                    m = search(node->child[i], t, beta, depth - 1, endTimeMs); // G4'
                }
            }
            if (m >= beta) {
                // beta cut-off
                transpositionTable->insert(node->board.hash, depth, m, FLAG_LOWERBOUND);
                return m;
            }
        }
        if (m > alpha) {
            transpositionTable->insert(node->board.hash, depth, m, FLAG_EXACT);
        }
        else {
            transpositionTable->insert(node->board.hash, depth, m, FLAG_UPPERBOUND);
        }
        return m;
    }
    else {
        // min node, algorithm G4'
        int m = INT_MAX, t; // m is current best upper bound, fail soft
        if (tableResult != nullptr) { m = tableResult->score; }
        m = std::min(m, search(node->child[0], alpha, beta, depth - 1, endTimeMs));
        if (m <= alpha) {
            // alpha cut-off
            transpositionTable->insert(node->board.hash, depth, m, FLAG_UPPERBOUND);
            return m;
        }
        for (int i = 1; i < node->numChild; ++i) {
            t = search(node->child[i], m - 1, m, depth - 1, endTimeMs); // null-window search
            if (t < m) {
                // failed low
                if (depth < 3 || t <= alpha) {
                    m = t;
                }
                else {
                    // alpha cut-off, re-search
                    m = search(node->child[i], alpha, t, depth - 1, endTimeMs);
                }
            }
            if (m <= alpha) {
                // alpha cut-off
                transpositionTable->insert(node->board.hash, depth, m, FLAG_UPPERBOUND);
                return m;
            }
        }
        if (m < beta) {
            transpositionTable->insert(node->board.hash, depth, m, FLAG_EXACT);
        }
        else {
            transpositionTable->insert(node->board.hash, depth, m, FLAG_LOWERBOUND);
        }
        return m;
    }
}



Move SecondStrategy::getMoveListFlip(Board* board, int turn) {
    if (board->numPieceUnflipped[board->turn] == 0) {
        // no more piece to flip!
        return Move(-1, -1);
    }
    // TODO: generate MoveList containing only one flip move that is the best
    int bestPos = -1, src = 0;
    int bestPosScore = INT_MIN, posScore = 0;
    int thisTypePieceNum = 0, color, type, turn_multiplier;
    for (int fidx = 0; fidx < 32; ++fidx) {
        src = flipSequence[fidx];
        if (board->block[src] == nullptr) {
            posScore = 0;
            // test if any surrounded ply Y can eat the ply I flipped X on this POS or vice versa.
            // if Y can eat X, we minus plyBasicScore[X] * nonFlipped[X] * turn_multiplier to POS_SCORE.
            // if X is ours, turn_multiplier is 1. If it is enemy's, turn_multiplier is -1.
            for (int pType = 1; pType < 16; ++pType) {
                if ((pType & 7) == 0) { continue; }
                color = pType >> 3;
                type = pType & 7;
                turn_multiplier = (color == turn)? 1 : -1;
                if ((thisTypePieceNum = allPieceTypeNum[type] - board->useableNumPiece[color][type])) {
                    // there are still PTYPE piece being dark.
                    
                    // by move
                    for (int didx = 0; didx < MOVE_NUM[src]; ++didx) {
                        int dst = src + MOVE_DIR[src][didx];
                        if (board->block[dst]!=nullptr && !(board->block[dst]->isDead)) {
                            // if dst->pieceType can eat this type
                            if (CAN_EAT_BY_MOVE[board->block[dst]->pieceType][pType]) {
                                posScore -= plyBasicScore[pType] * thisTypePieceNum * turn_multiplier;
                            }
                            // if this type can eat dst->pieceType
                            if (CAN_EAT_BY_MOVE[pType][board->block[dst]->pieceType]) {
                                posScore += plyBasicScore[board->block[dst]->pieceType] * thisTypePieceNum * turn_multiplier;
                            }
                        }
                    }

                    // by jump
                    for (int jidx = 0; jidx < JUMP_NUM[src]; ++jidx) {
                        bool existJumpedPiece = false;
                        for (int dst = src + JUMP_DIR[src][jidx]; !IS_OUT[dst]; dst += JUMP_DIR[src][jidx]) {
                            if (board->block[dst] == nullptr || !board->block[dst]->isDead) {
                                if (!existJumpedPiece) { existJumpedPiece = true; }
                                else if (board->block[dst] != nullptr) {
                                    // if dst->pieceType can jump-eat this type
                                    if (CAN_EAT_BY_JUMP[board->block[dst]->pieceType][pType]) {
                                        posScore -= plyBasicScore[pType] * thisTypePieceNum * turn_multiplier;
                                    }
                                    // if this can jump-eat dst->pieceType
                                    if (CAN_EAT_BY_JUMP[pType][board->block[dst]->pieceType]) {
                                        posScore += plyBasicScore[board->block[dst]->pieceType] * thisTypePieceNum * turn_multiplier;
                                    }
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
            
            fprintf(stdout, "getMoveListFlip::pos %d -> score %d\n", src, posScore);

            if (posScore > bestPosScore) {
                bestPosScore = posScore;
                bestPos = src;
            }
        }
    }
    fprintf(stdout, "getMoveListFlip::bestPos %d -> score %d\n", bestPos, posScore);
    return Move(bestPos, bestPos);
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
        for (int didx = 0; didx < MOVE_NUM[src]; ++didx) {
            int dst = src + MOVE_DIR[src][didx];
            if (board->block[dst]!=nullptr && board->block[dst]->isDead) {
                list.emplace_back(src, dst);
            }
        }
    }
    return list;
}