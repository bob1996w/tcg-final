#pragma once

#define LEFT -1
#define RIGHT 1
#define UP -10
#define DOWN 10

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


static bool CAN_EAT_BY_MOVE [16][16] = {
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,},
{0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,},
{0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,},
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,},
{0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,},
{0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,},
{0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,},
};

static bool CAN_EAT_BY_JUMP [16][16] = {
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
};

static int MOVE_NUM [60] = {
0,0,0,0,0,0,0,0,0,0,
0,2,3,3,3,3,3,3,2,0,
0,3,4,4,4,4,4,4,3,0,
0,3,4,4,4,4,4,4,3,0,
0,2,3,3,3,3,3,3,2,0,
0,0,0,0,0,0,0,0,0,0,
};
static int MOVE_DIR [60][4] = {
{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},
{0,0,0,0,},{1,10,0,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,10,0,0,},{0,0,0,0,},
{0,0,0,0,},{1,-10,10,0,},{-1,1,-10,10,},{-1,1,-10,10,},{-1,1,-10,10,},{-1,1,-10,10,},{-1,1,-10,10,},{-1,1,-10,10,},{-1,-10,10,0,},{0,0,0,0,},
{0,0,0,0,},{1,-10,10,0,},{-1,1,-10,10,},{-1,1,-10,10,},{-1,1,-10,10,},{-1,1,-10,10,},{-1,1,-10,10,},{-1,1,-10,10,},{-1,-10,10,0,},{0,0,0,0,},
{0,0,0,0,},{1,-10,0,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,-10,0,0,},{0,0,0,0,},
{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},
};
static int JUMP_NUM [60] = {
0,0,0,0,0,0,0,0,0,0,
0,2,2,3,3,3,3,2,2,0,
0,2,2,3,3,3,3,2,2,0,
0,2,2,3,3,3,3,2,2,0,
0,2,2,3,3,3,3,2,2,0,
0,0,0,0,0,0,0,0,0,0,
};
static int JUMP_DIR [60][4] = {
{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},
{0,0,0,0,},{1,10,0,0,},{1,10,0,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,10,0,0,},{-1,10,0,0,},{0,0,0,0,},
{0,0,0,0,},{1,10,0,0,},{1,10,0,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,1,10,0,},{-1,10,0,0,},{-1,10,0,0,},{0,0,0,0,},
{0,0,0,0,},{1,-10,0,0,},{1,-10,0,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,-10,0,0,},{-1,-10,0,0,},{0,0,0,0,},
{0,0,0,0,},{1,-10,0,0,},{1,-10,0,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,1,-10,0,},{-1,-10,0,0,},{-1,-10,0,0,},{0,0,0,0,},
{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},{0,0,0,0,},
};

static bool IS_OUT [60] = {
1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,
};

static int DIST [60][60] = {
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, },
{1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, },
{2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 3, 3, 3, 4, 5, 6, 7, 8, 9, 10, 4, 4, 4, 5, 6, 7, 8, 9, 10, 11, 5, 5, 5, 6, 7, 8, 9, 10, 11, 12, },
{3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 2, 2, 2, 2, 3, 4, 5, 6, 7, 8, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9, 4, 4, 4, 4, 5, 6, 7, 8, 9, 10, 5, 5, 5, 5, 6, 7, 8, 9, 10, 11, },
{4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 2, 2, 2, 2, 2, 3, 4, 5, 6, 7, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 4, 4, 4, 4, 4, 5, 6, 7, 8, 9, 5, 5, 5, 5, 5, 6, 7, 8, 9, 10, },
{5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 2, 2, 2, 2, 2, 2, 3, 4, 5, 6, 3, 3, 3, 3, 3, 3, 4, 5, 6, 7, 4, 4, 4, 4, 4, 4, 5, 6, 7, 8, 5, 5, 5, 5, 5, 5, 6, 7, 8, 9, },
{6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 2, 2, 2, 2, 2, 2, 2, 3, 4, 5, 3, 3, 3, 3, 3, 3, 3, 4, 5, 6, 4, 4, 4, 4, 4, 4, 4, 5, 6, 7, 5, 5, 5, 5, 5, 5, 5, 6, 7, 8, },
{7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, },
{8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, },
{9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, },
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, },
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, },
{3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 3, 3, 3, 4, 5, 6, 7, 8, 9, 10, 4, 4, 4, 5, 6, 7, 8, 9, 10, 11, },
{4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 2, 2, 2, 2, 3, 4, 5, 6, 7, 8, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9, 4, 4, 4, 4, 5, 6, 7, 8, 9, 10, },
{5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 2, 2, 2, 2, 2, 3, 4, 5, 6, 7, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 4, 4, 4, 4, 4, 5, 6, 7, 8, 9, },
{6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 2, 2, 2, 2, 2, 2, 3, 4, 5, 6, 3, 3, 3, 3, 3, 3, 4, 5, 6, 7, 4, 4, 4, 4, 4, 4, 5, 6, 7, 8, },
{7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 2, 2, 2, 2, 2, 2, 2, 3, 4, 5, 3, 3, 3, 3, 3, 3, 3, 4, 5, 6, 4, 4, 4, 4, 4, 4, 4, 5, 6, 7, },
{8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 6, },
{9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, },
{10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, },
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, },
{3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, },
{4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 3, 3, 3, 4, 5, 6, 7, 8, 9, 10, },
{5, 4, 3, 2, 2, 2, 2, 2, 2, 2, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 2, 2, 2, 2, 3, 4, 5, 6, 7, 8, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9, },
{6, 5, 4, 3, 2, 2, 2, 2, 2, 2, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 2, 2, 2, 2, 2, 3, 4, 5, 6, 7, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, },
{7, 6, 5, 4, 3, 2, 2, 2, 2, 2, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 2, 2, 2, 2, 2, 2, 3, 4, 5, 6, 3, 3, 3, 3, 3, 3, 4, 5, 6, 7, },
{8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 2, 2, 2, 2, 2, 2, 2, 3, 4, 5, 3, 3, 3, 3, 3, 3, 3, 4, 5, 6, },
{9, 8, 7, 6, 5, 4, 3, 2, 2, 2, 8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 5, },
{10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, },
{11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, },
{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, },
{4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, },
{5, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 2, 2, 2, 3, 4, 5, 6, 7, 8, 9, },
{6, 5, 4, 3, 3, 3, 3, 3, 3, 3, 5, 4, 3, 2, 2, 2, 2, 2, 2, 2, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, 2, 2, 2, 2, 3, 4, 5, 6, 7, 8, },
{7, 6, 5, 4, 3, 3, 3, 3, 3, 3, 6, 5, 4, 3, 2, 2, 2, 2, 2, 2, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 2, 2, 2, 2, 2, 3, 4, 5, 6, 7, },
{8, 7, 6, 5, 4, 3, 3, 3, 3, 3, 7, 6, 5, 4, 3, 2, 2, 2, 2, 2, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 2, 2, 2, 2, 2, 2, 3, 4, 5, 6, },
{9, 8, 7, 6, 5, 4, 3, 3, 3, 3, 8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 2, 2, 2, 2, 2, 2, 2, 3, 4, 5, },
{10, 9, 8, 7, 6, 5, 4, 3, 3, 3, 9, 8, 7, 6, 5, 4, 3, 2, 2, 2, 8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, },
{11, 10, 9, 8, 7, 6, 5, 4, 3, 3, 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, },
{12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, },
{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, },
{5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, },
{6, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 1, 1, 1, 2, 3, 4, 5, 6, 7, 8, },
{7, 6, 5, 4, 4, 4, 4, 4, 4, 4, 6, 5, 4, 3, 3, 3, 3, 3, 3, 3, 5, 4, 3, 2, 2, 2, 2, 2, 2, 2, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7, },
{8, 7, 6, 5, 4, 4, 4, 4, 4, 4, 7, 6, 5, 4, 3, 3, 3, 3, 3, 3, 6, 5, 4, 3, 2, 2, 2, 2, 2, 2, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, },
{9, 8, 7, 6, 5, 4, 4, 4, 4, 4, 8, 7, 6, 5, 4, 3, 3, 3, 3, 3, 7, 6, 5, 4, 3, 2, 2, 2, 2, 2, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, },
{10, 9, 8, 7, 6, 5, 4, 4, 4, 4, 9, 8, 7, 6, 5, 4, 3, 3, 3, 3, 8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, },
{11, 10, 9, 8, 7, 6, 5, 4, 4, 4, 10, 9, 8, 7, 6, 5, 4, 3, 3, 3, 9, 8, 7, 6, 5, 4, 3, 2, 2, 2, 8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, },
{12, 11, 10, 9, 8, 7, 6, 5, 4, 4, 11, 10, 9, 8, 7, 6, 5, 4, 3, 3, 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, },
{13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
{5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, },
{6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, },
{7, 6, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, },
{8, 7, 6, 5, 5, 5, 5, 5, 5, 5, 7, 6, 5, 4, 4, 4, 4, 4, 4, 4, 6, 5, 4, 3, 3, 3, 3, 3, 3, 3, 5, 4, 3, 2, 2, 2, 2, 2, 2, 2, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, },
{9, 8, 7, 6, 5, 5, 5, 5, 5, 5, 8, 7, 6, 5, 4, 4, 4, 4, 4, 4, 7, 6, 5, 4, 3, 3, 3, 3, 3, 3, 6, 5, 4, 3, 2, 2, 2, 2, 2, 2, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, },
{10, 9, 8, 7, 6, 5, 5, 5, 5, 5, 9, 8, 7, 6, 5, 4, 4, 4, 4, 4, 8, 7, 6, 5, 4, 3, 3, 3, 3, 3, 7, 6, 5, 4, 3, 2, 2, 2, 2, 2, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, },
{11, 10, 9, 8, 7, 6, 5, 5, 5, 5, 10, 9, 8, 7, 6, 5, 4, 4, 4, 4, 9, 8, 7, 6, 5, 4, 3, 3, 3, 3, 8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, },
{12, 11, 10, 9, 8, 7, 6, 5, 5, 5, 11, 10, 9, 8, 7, 6, 5, 4, 4, 4, 10, 9, 8, 7, 6, 5, 4, 3, 3, 3, 9, 8, 7, 6, 5, 4, 3, 2, 2, 2, 8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, },
{13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 12, 11, 10, 9, 8, 7, 6, 5, 4, 4, 11, 10, 9, 8, 7, 6, 5, 4, 3, 3, 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, },
{14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, },
};

static int BOARD_POSITION_SCORE [60] = {
0,0,0,0,0,0,0,0,0,0,
0,1,2,2,2,2,2,2,1,0,
0,2,3,3,4,4,3,3,2,0,
0,2,3,3,4,4,3,3,2,0,
0,1,2,2,2,2,2,2,1,0,
0,0,0,0,0,0,0,0,0,0,
};
