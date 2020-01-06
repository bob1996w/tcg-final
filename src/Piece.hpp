#pragma once
#include <stdio.h>
#include <string.h>

static char PIECE2CHAR[] = "_KGMRNCPXkgmrncp";
static int Char2Piece(char piece) {
    switch (piece) {
        case 'K':
            return 1;
            break;
        case 'G':
            return 2;
            break;
        case 'M':
            return 3;
            break;
        case 'R':
            return 4;
            break;
        case 'N':
            return 5;
            break;
        case 'C':
            return 6;
            break;
        case 'P':
            return 7;
            break;
        case 'k':
            return 9;
            break;
        case 'g':
            return 10;
            break;
        case 'm':
            return 11;
            break;
        case 'r':
            return 12;
            break;
        case 'n':
            return 13;
            break;
        case 'c':
            return 14;
            break;
        case 'p':
            return 15;
            break;
        default:
            return -1;
            break;
    }
}
class Piece {
public:
    int pos = -1; // pos = -1: still dark
    int listPos = -1; // position in board.piece
    int pieceType = 0;
    bool isDead = false;

    Piece();
    Piece(char type);
    Piece(int pType);
    void printPiece(char* message);
    void printSummary(char* message);
};