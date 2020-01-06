#include "Piece.hpp"

Piece::Piece() {
    pos = -1;
    listPos = -1;
    pieceType = 8;
    isDead = false;
}

Piece::Piece(char type) {
    pos = -1;
    listPos = -1;
    pieceType = Char2Piece(type);
    isDead = false;
}

Piece::Piece(int pType) {
    pos = -1;
    listPos = -1;
    pieceType = pType;
    isDead = false;
}

void Piece::printPiece(char* message) {
    if (isDead) {
        strcat(message, " - ");
    }
    else {
        char temp[5];
        sprintf(temp, " %c ", PIECE2CHAR[pieceType]);
        strcat(message, temp);
    }
}

void Piece::printSummary(char* message) {
    char temp[50]; // need enough space!
    sprintf(temp, "%c %2d pos: %3d isDead: %d listPos: %d\n", 
            PIECE2CHAR[pieceType], pieceType, pos, isDead, listPos);
    strcat(message, temp);
}