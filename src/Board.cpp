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
        useableNumPiece[TURN_RED][i] = 0;
        useableNumPiece[TURN_BLACK][i] = 0;
    }
    for (int i = 0; i < 16; i++) {
        piece[TURN_RED][i] = nullptr;
        piece[TURN_BLACK][i] = nullptr;
    }
    hash = 0LL;
    depth = 0;
    numPieceUnflipped[TURN_RED] = 16;
    numPieceUnflipped[TURN_BLACK] = 16;
}

void Board::copyBoard(Board& that) {
    for (int i = 0; i < 60; ++i) {
        block[i] = nullptr;
    }
    turn = that.turn;
    
    numPiece[TURN_RED] = that.numPiece[TURN_RED];
    numPiece[TURN_BLACK] = that.numPiece[TURN_BLACK];
    for (int i = 0; i < 16; ++i) {
        allPiece[TURN_RED][i] = Piece(that.allPiece[TURN_RED][i]);
        //printf("copy allPiece %d to %d\n", allPiece[TURN_RED][i].pos, that.allPiece[TURN_RED][i].pos);
        allPiece[TURN_BLACK][i] = Piece(that.allPiece[TURN_BLACK][i]);
    }
    for (int i = 1; i < 8; ++i) {
        flippedNumPiece[TURN_RED][i] = that.flippedNumPiece[TURN_RED][i];
        flippedNumPiece[TURN_BLACK][i] = that.flippedNumPiece[TURN_BLACK][i];
        useableNumPiece[TURN_RED][i] = that.useableNumPiece[TURN_RED][i];
        useableNumPiece[TURN_BLACK][i] = that.useableNumPiece[TURN_BLACK][i];
    }
    
    for (int i = 0; i < 16; ++i) {
        piece[TURN_RED][i] = nullptr;
        piece[TURN_BLACK][i] = nullptr;
    }
    hash = that.hash;
    depth = that.depth;
    numPieceUnflipped[TURN_RED] = that.numPieceUnflipped[TURN_RED];
    numPieceUnflipped[TURN_BLACK] = that.numPieceUnflipped[TURN_BLACK];

    // setup connections of Piece*
    for (int t = 0; t < 2; ++t) {
        for (int i = 0; i < 16; ++i) {
            Piece* p = &(allPiece[t][i]);
            if (p->pos >= 0) {
                block[p->pos] = p;
            }
            if (p->listPos >= 0) {
                piece[t][p->listPos] = p;
            }
        }
    }
}

void Board::applyMove(const char move[5]) {
    // first two chars are source coord, last two chars are destination coord.
    Move m((move[0] - 'a' + 1) * 10 + (move[1] - '0'), (move[2] - 'a' + 1) * 10 + (move[3] - '0'));
    applyMove(m);
}

void Board::applyMove(Move move) {
    //printBoard();
    int src = move.first, dst = move.second;
    //printf("move: %d-%d\n", src, dst);
    //fflush(stdout);
    Piece *srcPiece = block[src], *dstPiece = block[dst];
    if (dstPiece != nullptr) {
        // eat or to eaten space
        int swapListPos = dstPiece->listPos;
        dstPiece->pos = src;
        if (!dstPiece->isDead) {
            // remove this piece
            hash ^= ZOBRIST_KEY[dstPiece->pieceType][dst];
            dstPiece->isDead = true;
            dstPiece->listPos = -1;
            int color = (dstPiece->pieceType) >> 3;
            --numPiece[color];
            --useableNumPiece[color][dstPiece->pieceType & 7];
            if (swapListPos != numPiece[color]) {
                // move the last one on piece to current position
                piece[color][numPiece[color]]->listPos = swapListPos;
                piece[color][swapListPos] = piece[color][numPiece[color]];
            }
        }
        
    }
    hash ^= ZOBRIST_KEY[srcPiece->pieceType][src];
    hash ^= ZOBRIST_KEY[srcPiece->pieceType][dst];

    srcPiece->pos = dst;
    // cannot directly swap two pieces! we can only swap the pointer to board.
    block[src] = dstPiece;
    block[dst] = srcPiece;
    afterApplyAction();
}

void Board::applyFlip(const char move[4]) {
    int src = (move[0] - 'a' + 1) * 10 + (move[1] - '0');
    applyFlip(Flip(src, move[2]));
}

void Board::applyFlip(Flip flipMove) {
    // first two chars are coordinate, the third is the pieceType.
    int src = flipMove.first;
    int pChar = flipMove.second;
    //printf("flip: %d, %c\n", src, pChar);
    //fflush(stdout);
    int pType = Char2Piece(pChar);
    int color = pType >> 3;
    int type = pType & 7;
    //printf("type %d, color %d\n", type, color);
    //fflush(stdout);
    Piece* piecePtr = &(allPiece[color][allPieceTypePos[type] + flippedNumPiece[color][type]]);
    ++flippedNumPiece[color][type];
    ++useableNumPiece[color][type];
    --numPieceUnflipped[color];
    
    piecePtr->pos = src;
    piecePtr->listPos = numPiece[color];
    block[src] = piecePtr;
    piece[color][numPiece[color]] = piecePtr;
    ++numPiece[color];

    hash ^= ZOBRIST_KEY[pType][src];

    afterApplyAction();
}

// update the turn.
void Board::afterApplyAction() {
    depth += 1;
    if (turn == TURN_RED) { turn = TURN_BLACK; hash ^= ZOBRIST_TURN[TURN_RED] ^ ZOBRIST_TURN[TURN_BLACK]; }
    else if (turn == TURN_BLACK) { turn = TURN_RED; hash ^= ZOBRIST_TURN[TURN_RED] ^ ZOBRIST_TURN[TURN_BLACK]; }
    checkAndUpdateWinner();
}

// ============ utility ===========

// generate move list for the turn(color)
MoveList Board::getAllMoveList() {
    MoveList list;
    if (turn == TURN_UNKNOWN) {
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
        fprintf(stderr, "scanned flip\n");
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
        fprintf(stderr, "scanned move\n");
        fflush(stderr);
    }
    return list;
}

bool Board::checkAndUpdateWinner() {
    // TODO: implement winner checkint and update Board::winner
    if (numPieceUnflipped[TURN_RED] == 0 && numPiece[TURN_RED] == 0) { winner = TURN_BLACK; }
    if (numPieceUnflipped[TURN_BLACK] == 0 && numPiece[TURN_BLACK] == 0) { winner= TURN_RED; }
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
    strcat(mes, "useableNumPiece :");
    for (int i = 0; i < 8; ++i) {
        sprintf(temp, "%d ", useableNumPiece[TURN_RED][i]);
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
    strcat(mes, "useableNumPiece :");
    for (int i = 0; i < 8; ++i) {
        sprintf(temp, "%d ", useableNumPiece[TURN_BLACK][i]);
        strcat(mes, temp);
    }
    strcat(mes, "\n");
    printf("%s\n\n",mes);
}

void Board::printMoveList(MoveList list) {
    printf("____MOVE LIST____\n");
    for (int i = 0; i < list.size(); ++i) {
        Move m = list.at(i);
        printMove(m);
        printf("\n");
    }
    printf("total %d possible moves\n", (int)list.size());
    printf("____END  LIST____\n\n");
}

void Board::printMove(Move m) {
    if (m.first == m.second) {
        printf("  flip %d", m.first);
    }
    else {
        if (block[m.second] == nullptr) {
            printf("  move %d %c -> %d X", m.first, PIECE2CHAR[block[m.first]->pieceType], m.second);
        }
        else if (block[m.second]->isDead) {
            printf("  move %d %c -> %d _", m.first, PIECE2CHAR[block[m.first]->pieceType], m.second);
        }
        else {
            printf("  eat  %d %c -> %d %c", m.first, PIECE2CHAR[block[m.first]->pieceType], m.second, PIECE2CHAR[block[m.second]->pieceType]);
        }
    }
}