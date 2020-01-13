#include "EarlyGameStrategy.hpp"
#include "TreeNode.hpp"
#include "Board.hpp"
#include "SecondStrategy.hpp"

void EarlyGameStrategy::updateStrategy(TreeNode* node) {
    if (node->board.numPiece[TURN_RED] + node->board.numPiece[TURN_BLACK] >= 1) {
        node->setStrategy(new SecondStrategy());
        printf("!Set strategy to SecondStrategy\n");
    }
}

int EarlyGameStrategy::boardScore(Board* board) {
    return 0;
}

MoveList EarlyGameStrategy::getOrderedMoveList(Board* board) {
    return MoveList();
}

Move EarlyGameStrategy::genMove(TreeNode* node, int leftTimeMs) {
    // if it is initial, flip the first sequence
    int ourTurn = node->board.turn;
    if (ourTurn == TURN_INITIAL) {
        return Move(12, 12);
    }
    int theirTurn = (ourTurn == TURN_RED) ? TURN_BLACK : TURN_RED;
    // if there is only our piece, flip the piece by flipSequence.
    if (node->board.numPiece[theirTurn] == 0) {
        int seqIdx = 0;
        while (seqIdx < 32) {
            if (node->board.block[flipSequence[seqIdx]] == nullptr) {
                return Move(flipSequence[seqIdx], flipSequence[seqIdx]);
            }
            seqIdx++;
        }
    }
    // if there is only their piece, flip the piece that can attack them.
    else if (node->board.numPiece[ourTurn] == 0) {
        // search for all the possible flip
        bool areaMightBeAttacked[60] = {}; // false
        //bool areaMightCanAttack[60] = {}; // false
        // scan through all their pieces to see if we could be attacked
        for (int i = 0; i < node->board.numPiece[theirTurn]; ++i) {
            int src = node->board.piece[theirTurn][i]->pos;
            int pType = node->board.piece[theirTurn][i]->pieceType;
            if ((pType & 7) != 6) {
                // check move
                for (int didx = 0; didx < MOVE_NUM[src]; ++didx) {
                    int dst = src + MOVE_DIR[src][didx];
                    if (node->board.block[dst]==nullptr) {
                        // this is not fliiped, we have to mark this at might be attacked.
                        areaMightBeAttacked[dst] = true;
                    }
                }
            }
            else {
                // check jump for cannon
                for (int jidx = 0; jidx < JUMP_NUM[src]; ++jidx) {
                    bool existJumpedPiece = false;
                    for (int dst = src + JUMP_DIR[src][jidx]; !IS_OUT[dst]; dst += JUMP_DIR[src][jidx]) {
                        if (node->board.block[dst] == nullptr || !node->board.block[dst]->isDead) {
                            if (!existJumpedPiece){existJumpedPiece = true;}
                            else if (node->board.block[dst] == nullptr) {
                                areaMightBeAttacked[dst] = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        // select from flipSequence where not areaMightBeAttacked.
        int seqIdx = 0;
        int src = -1;
        int backupSrc = -1; // used when all area are being attacked, we still need to flip something.
        while (seqIdx < 32) {
            src = flipSequence[seqIdx];
            if (node->board.block[src] == nullptr) {
                if (backupSrc == -1) {
                    backupSrc = src;
                }
                if (!areaMightBeAttacked[src]) {
                    return Move(src, src);
                }
            }
            seqIdx++;
        }
        return Move(backupSrc, backupSrc);
    }
    // or if both exist...
    else {
        int ourKing = (ourTurn << 3) | PT_RK;
        // int theirKing = (theirTurn << 3) | PT_RK;
        // int theirPawn = (theirTurn << 3) | PT_RP;

        // our king escape
        if (node->board.useableNumPiece[ourTurn][ourKing] == 1) {
            int src = node->board.allPiece[ourTurn][allPieceTypePos[ourKing]].pos;
            for (int didx = 0; didx < MOVE_NUM[src]; ++didx) {
                int dst = src + MOVE_DIR[src][didx];
                if (!IS_OUT[dst] && node->board.block[dst] != nullptr &&
                        node->board.block[dst]->isDead) {
                    // is a blank that we can move to
                    // lookout for counters
                    bool canBeEat = false;
                    for (int didx2 = 0; didx2 < MOVE_NUM[didx2]; ++didx2) {
                        int dst2 = dst + MOVE_DIR[src][didx2];
                        if (!IS_OUT[dst2] && node->board.block[dst2] != nullptr && 
                                !node->board.block[dst2]->isDead) {
                            int dst2DType = node->board.block[dst2]->pieceType;
                            if (CAN_EAT_BY_MOVE[dst2DType][ourKing]) {
                                canBeEat = true;
                                break;
                            }
                        }
                        
                    }
                    if (canBeEat == true) {
                        continue;
                    }
                    // lookout for jumps
                    for (int didx2 = 0; didx2 < MOVE_NUM[didx2]; ++didx2) {
                        // not finished
                    }
                }
            }
        }
        // eat their king
        
        // escape ply

        // eat ply

    }
}

