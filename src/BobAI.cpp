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
    cleanedAfterGenMove = false;
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

void BobAI::cleanUp() {
    if (!cleanedAfterGenMove) {
        fprintf(stderr, "[[[attempt to free root]]]\n");fflush(stdout);fflush(stderr);
        NodePool::free(root);
        fprintf(stderr, "[[[successful free root]]]\n");fflush(stdout);fflush(stderr);
        cleanedAfterGenMove = true;
    }
}