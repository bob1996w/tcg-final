#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <random>
#include "Piece.hpp"
#include "CustomTypes.hpp"
#include "CDCRule.hpp"
#include "Utility.hpp"
#include "HashConstant.hpp"

#define TURN_RED 0
#define TURN_BLACK 1
#define TURN_UNKNOWN 2 
#define TURN_INITIAL 3

// [0] is unused
static int allPieceTypePos[] = {-1, 0, 1, 3, 5, 7, 9, 11};
static int allPieceTypeNum[] = { 0, 1, 2, 2, 2, 2, 2, 5};

class Board {
public:
    Piece* block[6 * 10];
    int turn = TURN_INITIAL;
    Piece* piece[2][16];
    int numPiece[2];                        // how many pieces are avaliable for each color? region for piece[color]
    int flippedNumPiece[2][8];              // how many pieces per type are already flipped for each color?
    int useableNumPiece[2][8];              // how many pieces per type are still on the board, not died?
    int numPieceUnflipped[2] = {16, 16};    // how many pieces are unflipped?
    Piece allPiece[2][16];
    Hash hash;
    int depth;
    int winner = TURN_INITIAL;      // TURN_INITIAL = winner not decided, TURN_UNKNOWN = DRAW

    void initBoard();
    void copyBoard(Board& that);
    void applyMove(const char move[5]);
    void applyMove(Move move);
    void applyFlip(const char move[4]);
    void applyFlip(Flip flipMove);
    void afterApplyAction();
    // ========== utility ===========
    MoveList getAllMoveList();
    bool checkAndUpdateWinner();
    // ========== display ==========
    void printBoard();
    void printBlock(char* message, int pos);
    void printAllPiece();
    void printMoveList(MoveList list);
    void printMove(Move m);
};