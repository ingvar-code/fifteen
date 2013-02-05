/* 
 * date: 18.01.2011
 * author: ingvar
 * 
 */
#include <stdio.h>
#include <stdlib.h>

/* COMMON VARIABLES */
#define SIDE 4
#define EMPTY 0

enum TURN {
    NONE,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct CELL {
    int row;
    int col;
};

struct STEP {
    struct STEP* prev; //previous step
    int count; //previous steps
    int rate; //less is better
    enum TURN turn; //which side was turn
    int state[SIDE][SIDE]; //current state(after turn)
};

struct STAT {
    struct STEP* solve;
    int explored;
    int seconds;
    int speed; //states per second
};

/* COMMON FUNCTIONS */
#define MAX(N1, N2) (N1 > N2 ? N1 : N2)
#define MIN(N1, N2) (N1 < N2 ? N1 : N2)
#define ABS(N) (N >= 0 ? N : (N) * -1)

#define correct_num(ROW, COL) (SIDE * (ROW) + (COL) + 1)

struct CELL* correct_pos(int);
int moves_count(int, int, int);
