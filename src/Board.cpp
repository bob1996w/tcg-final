#include "Board.hpp"

void Board::initBoard() {
    for (int i = 0; i < 60; ++i) {
        block[i] = nullptr;
    }
    turn = TURN_INITIAL;
    numPiece[TURN_RED] = 0;
    numPiece[TURN_BLACK] = 0;
    for (int i = 1; i < 8; ++i) {
        for (int j = 0; j < allPieceTypeNum[i]; ++j) {
            allPiece[TURN_RED][allPieceTypePos[i] + j] = Piece(i);
            allPiece[TURN_BLACK][allPieceTypePos[i] + j] = Piece(i | 8);
        }
        flippedNumPiece[TURN_RED][i] = 0;
        flippedNumPiece[TURN_BLACK][i] = 0;
    }
    for (int i = 0; i < 16; i++) {
        piece[TURN_RED][i] = nullptr;
        piece[TURN_BLACK][i] = nullptr;
    }
}

void Board::genMove(char move[6]) {
    printBoard();
    MoveList list = getAllMoveList(turn);
    printMoveList(list);
    int idx = Utility::getRandomNum((int)list.size());
    printf("genMove get idx %d in list size %d\n", idx, list.size());
    fflush(stdout);
    Move m = list.at(idx);
    move[0] = m.first / 10 - 1 + 'a';
    move[1] = m.first % 10 + '0';
    move[2] = ' ';
    move[3] = m.second / 10 - 1 + 'a';
    move[4] = m.second % 10 + '0';
    move[5] = '\0';
    printf("genMove: %s\n", move);
}

void Board::applyMove(const char move[5]) {
    printBoard();
    // first two chars are source coord, last two chars are destination coord.
    int src, dst;
    src = (move[0] - 'a' + 1) * 10 + (move[1] - '0');
    dst = (move[2] - 'a' + 1) * 10 + (move[3] - '0');
    printf("move: %d-%d\n", src, dst);
    fflush(stdout);
    Piece *srcPiece = block[src], *dstPiece = block[dst];
    if (dstPiece != nullptr) {
        // eat or to eaten space
        int swapListPos = dstPiece->listPos;
        dstPiece->pos = src;
        if (!dstPiece->isDead) {
            dstPiece->isDead = true;
            dstPiece->listPos = -1;
            int color = (dstPiece->pieceType) >> 3;
            --numPiece[color];
            if (swapListPos != numPiece[color]) {
                // move the last one on piece to current position
                piece[color][numPiece[color]]->listPos = swapListPos;
                piece[color][swapListPos] = piece[color][numPiece[color]];
            }
        }
        
    }
    srcPiece->pos = dst;
    // cannot directly swap two pieces! we can only swap the pointer to board.
    block[src] = dstPiece;
    block[dst] = srcPiece;
    afterApplyAction();
}

void Board::applyFlip(const char move[4]) {
    // first two chars are coordinate, the third is the pieceType.
    int src;
    src = (move[0] - 'a' + 1) * 10 + (move[1] - '0');
    printf("flip: %d, %c\n", src, move[2]);
    fflush(stdout);
    int pType = Char2Piece(move[2]);
    int color = pType >> 3;
    int type = pType & 7;
    printf("type %d, color %d\n", type, color);
    fflush(stdout);
    Piece* piecePtr = &(allPiece[color][allPieceTypePos[type] + flippedNumPiece[color][type]]);
    ++flippedNumPiece[color][type];
    
    piecePtr->pos = src;
    piecePtr->listPos = numPiece[color];
    block[src] = piecePtr;
    piece[color][numPiece[color]] = piecePtr;
    ++numPiece[color];
    afterApplyAction();
}

// update the turn.
void Board::afterApplyAction() {
    if (turn == TURN_RED) { turn = TURN_BLACK; }
    else if (turn == TURN_BLACK) { turn = TURN_RED; }
}

// ============ utility ===========

// generate move list for the turn(color)
MoveList Board::getAllMoveList(int turn) {
    MoveList list;
    if (turn == TURN_INITIAL) {
        // only flip
        for (int i = 0; i < 60; ++i) {
            if (!IS_OUT[i]) {
                list.emplace_back(i, i);
            }
        }
    }
    else {
        // flip
        for (int i = 0; i < 60; ++i) {
            if ((!IS_OUT[i]) && block[i] == nullptr) {
                list.emplace_back(i, i);
            }
        }
        fprintf(stderr, "scanned flip");
        fflush(stderr);
        // move
        for (int pidx = 0; pidx < numPiece[turn]; ++pidx) {
            Piece* pc = piece[turn][pidx];
            int src = pc->pos;
            int pType = pc->pieceType;
            for (int didx = 0; didx < MOVE_NUM[src]; ++didx) {
                int dst = src + MOVE_DIR[src][didx];
                if (block[dst] != nullptr) {
                    fprintf(stdout, "    src %d, dst %d\n", src, dst);
                    if (block[dst]->isDead) {
                        // dst is empty
                        list.emplace_back(src, dst);
                    }
                    else {
                        // eat?
                        if (CAN_EAT_BY_MOVE[pType][block[dst]->pieceType]) {
                            list.emplace_back(src, dst);
                        }
                    }
                }
            }
            // cannon jump
            if ((pType & 7) == 6) {
                for (int jidx = 0; jidx < JUMP_NUM[src]; ++jidx) {
                    bool existJumpedPiece = false;
                    for (int dst = src + JUMP_DIR[src][jidx]; !IS_OUT[dst]; dst += JUMP_DIR[src][jidx]) {
                        if (block[dst] == nullptr || !block[dst]->isDead) {
                            if (!existJumpedPiece) {existJumpedPiece = true;}
                            else if (block[dst] != nullptr && CAN_EAT_BY_JUMP[pType][block[dst]->pieceType]) {
                                list.emplace_back(src, dst);
                                break;
                            }
                            else {
                                // jump destination is dark or cannot eat, cannot jump
                                break;
                            }
                        }
                    }
                }
            }
        }
        fprintf(stderr, "scanned move");
        fflush(stderr);
    }
    return list;
}

// ============ display ==========
void Board::printBoard() {
    printAllPiece();
    char mes[1024] = "";
    char temp[1024];
    char myColor[10] = "";
    fflush(stdout);
    if (turn == TURN_RED) {
        strcpy(myColor, "Red");
    }
    else if (turn == TURN_BLACK) {
        strcpy(myColor, "Black");
    }
    else {
        strcpy(myColor, "Unknown");
    }
    sprintf(temp, "----------%s---------\n", myColor);
    strcat(mes, temp);
    for (int i = 0; i < 60; ++i) {
        if (i % 10 == 0) {
            sprintf(temp, "\n<%d>", i / 10);
            strcat(mes, temp);
        }
        printBlock(mes, i);
    }
    strcat(mes, "\n\n   ");
    for (int i = 0; i < 10; ++i) {
        sprintf(temp, "<%d>", i);
        strcat(mes, temp);
    }
    printf("%s\n\n", mes);
}

void Board::printBlock(char* message, int pos) {
    if (IS_OUT[pos]) {
        strcat(message, " . ");
    }
    else if (block[pos] == nullptr) {
        // not flipped, they are dark pieces.
        strcat(message, " X ");
    }
    else {
        block[pos]->printPiece(message);
    }
}

void Board::printAllPiece() {
    char mes[4096] = "";
    char temp[1024];
    strcat(mes, "======= all pieces =======\n");
    strcat(mes, "===red===\n");
    for (int i = 0; i < 16; ++i) {
        allPiece[TURN_RED][i].printSummary(mes);
    }
    strcat(mes, "---pieces red---\n");
    for (int i = 0; i < numPiece[TURN_RED]; ++i) {
        piece[TURN_RED][i]->printSummary(mes);
    }
    strcat(mes, "flippedNumPiece :");
    for (int i = 0; i < 8; ++i) {
        sprintf(temp, "%d ", flippedNumPiece[TURN_RED][i]);
        strcat(mes, temp);
    }
    strcat(mes, "\n");
    strcat(mes, "===black===\n");
    for (int i = 0; i < 16; ++i) {
        allPiece[TURN_BLACK][i].printSummary(mes);
    }
    strcat(mes, "---pieces black---\n");
    for (int i = 0; i < numPiece[TURN_BLACK]; ++i) {
        piece[TURN_BLACK][i]->printSummary(mes);
    }
    strcat(mes, "flippedNumPiece :");
    for (int i = 0; i < 8; ++i) {
        sprintf(temp, "%d ", flippedNumPiece[TURN_BLACK][i]);
        strcat(mes, temp);
    }
    strcat(mes, "\n");
    printf("%s\n\n",mes);
}

void Board::printMoveList(MoveList list) {
    printf("____MOVE LIST____\n");
    for (int i = 0; i < list.size(); ++i) {
        Move m = list.at(i);
        if (m.first == m.second) {
            printf("  flip %d\n", m.first);
        }
        else {
            if (block[m.second] == nullptr) {
                printf("  move %d %c -> %d X\n", m.first, PIECE2CHAR[block[m.first]->pieceType], m.second);
            }
            printf("  move %d %c -> %d %c\n", m.first, PIECE2CHAR[block[m.first]->pieceType], m.second, PIECE2CHAR[block[m.second]->pieceType]);
        }
    }
    printf("total %d possible moves\n", list.size());
    printf("____END  LIST____\n\n");
}