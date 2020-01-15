#include <iostream>
#include <string>
#include <random>
#include <cmath>

using namespace std;

bool is_out[60];
int move_num [60];
int move_dir [60][4];
int jump_num [60];
int jump_dir [60][4];

#define LEFT -1
#define RIGHT 1
#define UP -10
#define DOWN 10

// [y][x] move from y to x, can y eat x?
bool canEatTypeByMove [8][8] = {
    /*  -   K   G   M   R   N   C   P   */
/*-*/{  0,  0,  0,  0,  0,  0,  0,  0,},
/*K*/{  0,  1,  1,  1,  1,  1,  1,  0,},
/*G*/{  0,  0,  1,  1,  1,  1,  1,  1,},
/*M*/{  0,  0,  0,  1,  1,  1,  1,  1,},
/*R*/{  0,  0,  0,  0,  1,  1,  1,  1,},
/*N*/{  0,  0,  0,  0,  0,  1,  1,  1,},
/*C*/{  0,  0,  0,  0,  0,  0,  0,  0,},
/*P*/{  0,  1,  0,  0,  0,  0,  0,  1,},
};

// can cannon jump to eat x?
bool canEatTypeByJump [8][8] = {
    /*  -   K   G   M   R   N   C   P   */
/*-*/{  0,  0,  0,  0,  0,  0,  0,  0,},
/*K*/{  0,  0,  0,  0,  0,  0,  0,  0,},
/*G*/{  0,  0,  0,  0,  0,  0,  0,  0,},
/*M*/{  0,  0,  0,  0,  0,  0,  0,  0,},
/*R*/{  0,  0,  0,  0,  0,  0,  0,  0,},
/*N*/{  0,  0,  0,  0,  0,  0,  0,  0,},
/*C*/{  0,  1,  1,  1,  1,  1,  1,  1,},
/*P*/{  0,  0,  0,  0,  0,  0,  0,  0,},
};

// small score additions when piece-eating cannot tell difference
// also to prevent from wandering
int boardPositionScore [60] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 2, 2, 2, 2, 2, 1, 0,
    0, 2, 3, 3, 4, 4, 3, 3, 2, 0,
    0, 2, 3, 3, 4, 4, 3, 3, 2, 0,
    0, 1, 2, 2, 2, 2, 2, 2, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

int main () {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (i == 0 || i == 5 || j == 0 || j == 9) {
                is_out[i * 10 + j] = true;
            }
            else {
                is_out[i * 10 + j] = false;
            }
        }
    }

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 10; ++j) {
            int idx = i * 10 + j;
            if (is_out[idx]) {
                move_num[idx] = 0;
                jump_num[idx] = 0;
                for (int k = 0; k < 4; k++) {
                    move_dir[idx][k] = 0;
                    jump_dir[idx][k] = 0;
                }
            }
            else {
                int movedirnum = 0;
                int jumpdirnum = 0;
                if (!is_out[idx + LEFT]) {
                    move_dir[idx][movedirnum] = LEFT;
                    movedirnum++;
                    if (!is_out[idx + LEFT + LEFT]) {
                        jump_dir[idx][jumpdirnum] = LEFT;
                        jumpdirnum++;
                    }
                }
                if (!is_out[idx + RIGHT]) {
                    move_dir[idx][movedirnum] = RIGHT;
                    movedirnum++;
                    if (!is_out[idx + RIGHT + RIGHT]) {
                        jump_dir[idx][jumpdirnum] = RIGHT;
                        jumpdirnum++;
                    }
                }
                if (!is_out[idx + UP]) {
                    move_dir[idx][movedirnum] = UP;
                    movedirnum++;
                    if (!is_out[idx + UP + UP]) {
                        jump_dir[idx][jumpdirnum] = UP;
                        jumpdirnum++;
                    }
                }
                if (!is_out[idx + DOWN]) {
                    move_dir[idx][movedirnum] = DOWN;
                    movedirnum++;
                    if (!is_out[idx + DOWN + DOWN]) {
                        jump_dir[idx][jumpdirnum] = DOWN;
                        jumpdirnum++;
                    }
                }
                for (int k = movedirnum; k < 4; k++) {
                    move_dir[idx][k] = 0;
                }
                for (int k = jumpdirnum; k < 4; k++) {
                    jump_dir[idx][k] = 0;
                }
                move_num[idx] = movedirnum;
                jump_num[idx] = jumpdirnum;
            }
        }
    }

    cout << "#pragma once\n\n#define LEFT -1\n#define RIGHT 1\n#define UP -10\n#define DOWN 10\n";
    
    // pieceType constants
    std::string pieceTypeConstants = R"(
// pieceType constants

 // 紅帥
#define PT_RK  1
 // 紅仕
#define PT_RG  2
 // 紅相
#define PT_RM  3
 // 紅俥
#define PT_RR  4
 // 紅馬
#define PT_RN  5
 // 紅炮
#define PT_RC  6
 // 紅兵
#define PT_RP  7
 // 黑將
#define PT_BK  9
 // 黑士
#define PT_BG 10
 // 黑象
#define PT_BM 11
 // 黑車
#define PT_BR 12
 // 黑馬
#define PT_BN 13
 // 黑包
#define PT_BC 14
 // 黑卒
#define PT_BP 15

// all piece type array
static int ALL_PIECE_TYPE[14] = {1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15};
)";
    cout << pieceTypeConstants << endl;

    cout << endl;
    // check if a pieceType can eat another pieceType
    cout << "static bool CAN_EAT_BY_MOVE [16][16] = {" << endl;
    for (int i = 0; i < 16; ++i) {
        cout << "{";
        for (int j = 0; j < 16; ++j) {
            bool canEat = false;
            if (i >> 3 != j >> 3) {
                canEat = canEatTypeByMove[i & 7][j & 7];
            }
            if (canEat) {
                cout << "1,";
            }
            else {
                cout << "0,";
            }
        }
        cout << "}," << endl;
    }
    cout << "};" << endl;

    cout << endl;
    // check if a pieceType can jump to eat another pieceType
    cout << "static bool CAN_EAT_BY_JUMP [16][16] = {" << endl;
    for (int i = 0; i < 16; ++i) {
        cout << "{";
        for (int j = 0; j < 16; ++j) {
            bool canEat = false;
            if (i >> 3 != j >> 3) {
                canEat = canEatTypeByJump[i & 7][j & 7];
            }
            if (canEat) {
                cout << "1,";
            }
            else {
                cout << "0,";
            }
        }
        cout << "}," << endl;
    }
    cout << "};" << endl;

    cout << endl;
    // check ways to move and direction
    cout << "static int MOVE_NUM [60] = {" << endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 10; ++j) {
            int idx = i * 10 + j;
            cout << move_num[idx] << ",";
        }
        cout << endl;
    }
    cout << "};" << endl;

    cout << "static int MOVE_DIR [60][4] = {" << endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 10; ++j) {
            int idx = i * 10 + j;
            cout << "{";
            for (int k = 0; k < 4; ++k) {
                cout << move_dir[idx][k] << ",";
            }
            cout << "},";
        }
        cout << endl;
    }
    cout << "};" << endl;
    
    // check ways to jump and direction for cannon
    cout << "static int JUMP_NUM [60] = {" << endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 10; ++j) {
            int idx = i * 10 + j;
            cout << jump_num[idx] << ",";
        }
        cout << endl;
    }
    cout << "};" << endl;

    cout << "static int JUMP_DIR [60][4] = {" << endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 10; ++j) {
            int idx = i * 10 + j;
            cout << "{";
            for (int k = 0; k < 4; ++k) {
                cout << jump_dir[idx][k] << ",";
            }
            cout << "},";
        }
        cout << endl;
    }
    cout << "};" << endl;

    cout << endl;
    // check if there the block is out of the board region
    cout << "static bool IS_OUT [60] = {" << endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (is_out[i * 10 + j]) {
                cout << "1,";
            }
            else {
                cout << "0,";
            } 
        }
        cout << endl;
    }
    cout << "};" << endl;

    

    cout << endl;
    cout << "static int DIST [60][60] = {" << endl;
    for (int src = 0; src < 60; ++src) {
        cout << "{";
        for (int dst = 0; dst < 60; ++dst) {
            int t1 = (src < dst)? src: dst;
            int t2 = (src < dst)? dst: src;
            int yDist = 0, xDist = 0;
            while (t1/10 < t2/10) {
                ++yDist;
                t1 += DOWN;
            }
            while (t1 < t2) {
                ++xDist;
                t1 += RIGHT;
            }
            cout << yDist + xDist << ", ";
        }
        cout << "}," << endl;
    }
    cout << "};" << endl;

    cout << endl;
    cout << "static int BOARD_POSITION_SCORE [60] = {" << endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 10; ++j) {
            int idx = i * 10 + j;
            cout << boardPositionScore[idx] << ",";
        }
        cout << endl;
    }
    cout << "};" << endl;
}