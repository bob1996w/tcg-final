#include "BobAI.hpp"

BobAI::BobAI(void) { 
    this->Color = 2;
    transpositionTable = new HashMap();
    transpositionTable->initialize();
}

BobAI::~BobAI(void) {}

bool BobAI::protocol_version(const char* data[], char* response) {
    strcpy(response, "1.0.0");
    return 0;
}

bool BobAI::name(const char* data[], char* response) {
    strcpy(response, "BobAI");
    return 0;
}

bool BobAI::version(const char* data[], char* response) {
    strcpy(response, "1.0.0");
    return 0;
}

bool BobAI::known_command(const char* data[], char* response) {
    for (int i = 0; i < COMMAND_NUM; i++) {
        if (!strcmp(data[0], commands_name[i])) {
            strcpy(response, "true");
            return 0;
        }
    }
    strcpy(response, "false");
    return 0;
}

bool BobAI::list_commands(const char* data[], char* response) {
    for (int i = 0; i < COMMAND_NUM; i++) {
        strcat(response, commands_name[i]);
        if (i < COMMAND_NUM - 1) {
            strcat(response, "\n");
        }
    }
    return 0;
}

bool BobAI::quit(const char* data[], char* response) {
    fprintf(stderr, "Bye\n");
    quitGame = true;
    return 0;
}

bool BobAI::boardsize(const char* data[], char* response) {
    fprintf(stderr, "BoardSize: %s x %s\n", data[0], data[1]);
    return 0;
}

bool BobAI::reset_board(const char* data[], char* response) {
    // this->Color = 2;
    // this->Red_Time = -1;    // known
    // this->Black_Time = -1;  // known
    // this->initBoardState();
    myBoard.initBoard();
    return 0;
}

bool BobAI::num_repetition(const char* data[], char* response) {
    return 0;
}

bool BobAI::num_moves_to_draw(const char* data[], char* response) {
    return 0;
}

bool BobAI::move(const char* data[], char* response) {
    char move[5];
    sprintf(move, "%s%s", data[0], data[1]);
    fprintf(stdout, "bobai receive move: %s\n", move);
    fflush(stdout);
    myBoard.applyMove(move);
    return 0;
}

bool BobAI::flip(const char* data[], char* response) {
    char move[4];
    sprintf(move, "%s%s", data[0], data[1]);
    myBoard.applyFlip(move);
    return 0;
}

bool BobAI::genmove(const char* data[], char* response) {
    fprintf(stdout, "BobAI::genmove!\n");
    fflush(stdout);
    if (myBoard.turn == TURN_INITIAL || myBoard.turn == TURN_UNKNOWN) {
        // haven't get move or genmove yet
        // we won't be sure the board's current turn until first 
        // genmove "color" (not unknown) arrives
        if (!strcmp(data[0], "red")) {
            myBoard.turn = TURN_RED;
            myBoard.hash ^= ZOBRIST_TURN[TURN_RED];
        } else if (!strcmp(data[0], "black")) {
            myBoard.turn = TURN_BLACK;
            myBoard.hash ^= ZOBRIST_TURN[TURN_BLACK];
        } else {
            myBoard.turn = TURN_UNKNOWN;
        }
    }
    fprintf(stdout, "attempt to delete root\n");fflush(stdout);fflush(stderr);
    NodePool::free(root);
    fprintf(stdout, "successful delete root\n");fflush(stdout);fflush(stderr);
    char move[6];
    root = NodePool::alloc();
    root->setUpTreeNode(nullptr, myBoard, transpositionTable);
#ifdef DEBUG
    root -> dewey_part = "root";
#endif
    int timeLimit = Red_Time;
    if (root->board.turn == TURN_BLACK) {
        timeLimit = Black_Time;
    }
    root->genMove(move, timeLimit);
    sprintf(response, "%c%c %c%c", move[0], move[1], move[3], move[4]);
    return 0;
}

bool BobAI::game_over(const char* data[], char* response) {
    fprintf(stderr, "Game Result: %s\n", data[0]);
    return 0;
}

bool BobAI::ready(const char* data[], char* response) {
    return 0;
}

bool BobAI::time_settings(const char* data[], char* response) {
    return 0;
}

bool BobAI::time_left(const char* data[], char* response) {
    if (!strcmp(data[0], "red")) {
        sscanf(data[1], "%d", &(this->Red_Time));
    } else {
        sscanf(data[1], "%d", &(this->Black_Time));
    }
    fprintf(stderr, "Time Left(%s): %s\n", data[0], data[1]);
    return 0;
}

bool BobAI::showboard(const char* data[], char* response) {
    // Pirnf_Chessboard();
    myBoard.printBoard();
    return 0;
}

int GetFin(char c) {
    static const char skind[] = {'-', 'K', 'G', 'M', 'R', 'N', 'C', 'P', 'X', 'k', 'g', 'm', 'r', 'n', 'c', 'p'};
    for (int f = 0; f < 16; f++)
        if (c == skind[f]) return f;
    return -1;
}

//---------------------------------------------my search function---------------------------------------------

int BobAI::Expand(int* Board, int color, int* Result) {
    int ResultCount = 0;
    for (int i = 0; i < 32; i++) {
        if (Board[i] >= 0 && Board[i] / 7 == color) {
            //Gun
            if (Board[i] % 7 == 1) {
                int row = i / 4;
                int col = i % 4;
                for (int rowCount = row * 4; rowCount < (row + 1) * 4; rowCount++) {
                    if (Referee(Board, i, rowCount, color)) {
                        Result[ResultCount] = i * 100 + rowCount;
                        ResultCount++;
                    }
                }
                for (int colCount = col; colCount < 32; colCount += 4) {
                    if (Referee(Board, i, colCount, color)) {
                        Result[ResultCount] = i * 100 + colCount;
                        ResultCount++;
                    }
                }
            } else {
                int Move[4] = {i - 4, i + 1, i + 4, i - 1};
                for (int k = 0; k < 4; k++) {
                    if (Move[k] >= 0 && Move[k] < 32 && Referee(Board, i, Move[k], color)) {
                        Result[ResultCount] = i * 100 + Move[k];
                        ResultCount++;
                    }
                }
            }
        };
    }
    return ResultCount;
}

//---------------------------- Display --------------------------------


// Referee
bool BobAI::Referee(int* chess, int from_location_no, int to_location_no, int UserId) {
    int MessageNo = 0;
    bool IsCurrent = true;
    int from_chess_no = chess[from_location_no];
    int to_chess_no = chess[to_location_no];
    int from_row = from_location_no / 4;
    int to_row = to_location_no / 4;
    int from_col = from_location_no % 4;
    int to_col = to_location_no % 4;

    if (from_chess_no < 0 || (to_chess_no < 0 && to_chess_no != CHESS_EMPTY)) {
        MessageNo = 1;
        //strcat(Message,"**no chess can move**");
        //strcat(Message,"**can't move darkchess**");
        IsCurrent = false;
    } else if (from_chess_no >= 0 && from_chess_no / 7 != UserId) {
        MessageNo = 2;
        //strcat(Message,"**not my chess**");
        IsCurrent = false;
    } else if ((from_chess_no / 7 == to_chess_no / 7) && to_chess_no >= 0) {
        MessageNo = 3;
        //strcat(Message,"**can't eat my self**");
        IsCurrent = false;
    }
    //check attack
    else if (to_chess_no == CHESS_EMPTY && abs(from_row - to_row) + abs(from_col - to_col) == 1)  //legal move
    {
        IsCurrent = true;
    } else if (from_chess_no % 7 == 1)  //judge gun
    {
        int row_gap = from_row - to_row;
        int col_gap = from_col - to_col;
        int between_Count = 0;
        //slant
        if (from_row - to_row == 0 || from_col - to_col == 0) {
            //row
            if (row_gap == 0) {
                for (int i = 1; i < abs(col_gap); i++) {
                    int between_chess;
                    if (col_gap > 0)
                        between_chess = chess[from_location_no - i];
                    else
                        between_chess = chess[from_location_no + i];
                    if (between_chess != CHESS_EMPTY)
                        between_Count++;
                }
            }
            //column
            else {
                for (int i = 1; i < abs(row_gap); i++) {
                    int between_chess;
                    if (row_gap > 0)
                        between_chess = chess[from_location_no - 4 * i];
                    else
                        between_chess = chess[from_location_no + 4 * i];
                    if (between_chess != CHESS_EMPTY)
                        between_Count++;
                }
            }

            if (between_Count != 1) {
                MessageNo = 4;
                //strcat(Message,"**gun can't eat opp without between one piece**");
                IsCurrent = false;
            } else if (to_chess_no == CHESS_EMPTY) {
                MessageNo = 5;
                //strcat(Message,"**gun can't eat opp without between one piece**");
                IsCurrent = false;
            }
        }
        //slide
        else {
            MessageNo = 6;
            //strcat(Message,"**cant slide**");
            IsCurrent = false;
        }
    } else  // non gun
    {
        //judge pawn or king

        //distance
        if (abs(from_row - to_row) + abs(from_col - to_col) > 1) {
            MessageNo = 7;
            //strcat(Message,"**cant eat**");
            IsCurrent = false;
        }
        //judge pawn
        else if (from_chess_no % 7 == 0) {
            if (to_chess_no % 7 != 0 && to_chess_no % 7 != 6) {
                MessageNo = 8;
                //strcat(Message,"**pawn only eat pawn and king**");
                IsCurrent = false;
            }
        }
        //judge king
        else if (from_chess_no % 7 == 6 && to_chess_no % 7 == 0) {
            MessageNo = 9;
            //strcat(Message,"**king can't eat pawn**");
            IsCurrent = false;
        } else if (from_chess_no % 7 < to_chess_no % 7) {
            MessageNo = 10;
            //strcat(Message,"**cant eat**");
            IsCurrent = false;
        }
    }
    return IsCurrent;
}
