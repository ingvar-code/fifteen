/*
 * date: 18.01.2011
 * author: ingvar
 * 
 * State support functions
 * 
 */
#include "fifteen.h"

struct CELL* correct_pos(int num)
{
    struct CELL* pos = (struct CELL *) malloc(sizeof(struct CELL));
    pos->row = (num - 1) / SIDE;
    pos->col = (num % SIDE) - 1;
    return pos;
};

int moves_count(int num, int row, int col)
{
    if(num == EMPTY) {
        return 0;
    }
    int crow = (num - 1) / SIDE;
    int ccol = (num % SIDE) - 1;
    return ABS(row - crow) + ABS(col - ccol);
};

struct CELL* get_empty(int state[SIDE][SIDE])
{
    struct CELL* empty = NULL;
    for(int i = 0; i < SIDE; i++) {
        for(int j = 0; j < SIDE; j++) {
            if(state[i][j] == EMPTY) {
                empty = (struct CELL *) malloc(sizeof(struct CELL));
                empty->row = i;
                empty->col = j;
                goto end;
            }
        }
    }
    end:
    return empty;
};

void copy_states(int source[SIDE][SIDE], int receiver[SIDE][SIDE])
{
    for(int i = 0; i < SIDE; i++) {
        for(int j = 0; j < SIDE; j++) {
            receiver[i][j] = source[i][j];
        }
    }
};

short int equals_states(int s1[SIDE][SIDE], int s2[SIDE][SIDE])
{
    for(int i = 0; i < SIDE; i++) {
        for(int j = 0; j < SIDE; j++) {
            if(s1[i][j] != s2[i][j]) {
                return 0;
            }
        }
    }
    return 1;
};

void show_state(int state[SIDE][SIDE], enum TURN turn)
{
    for(int i = 0; i < SIDE; i++) {
        for(int j = 0; j < SIDE; j++) {
            if(state[i][j] == EMPTY) {
                char* arrow;
                switch (turn) {
                    case UP:
                        arrow = "-<";
                        break;
                    case RIGHT:
                        arrow = "->";
                        break;
                    case DOWN:
                        arrow = ">-";
                        break;
                    case LEFT:
                        arrow = "<-";
                        break;
                    default:
                        arrow = "  ";
                        break;
                }
                printf("%s ", arrow);
            }
            else if(state[i][j] == correct_num(i, j)) {
                printf("## ");
            }
            else {
                printf("%2d ", state[i][j]);
            }
        }
        putchar('\n');
    }
};

short int is_solved_state(int state[SIDE][SIDE])
{
    int N = 0;
    for(int row = 0; row < SIDE; row++) {
        for(int col = 0; col < SIDE; col++) {
            int num = state[row][col];
            if(num == EMPTY) {
                continue;
            }
            int k = 0;
            int j = col+1;
            for(int i = row; i < SIDE; i++) {
                for(; j < SIDE; j++) {
                    if(state[i][j] != EMPTY && state[i][j] < num) {
                        k++;
                    }
                }
                j = 0;
            }
            N += k;
        }
    }
    //find empty row
    for(int row = 0; row < SIDE; row++) {
        for(int col = 0; col < SIDE; col++) {
            if(state[row][col] == EMPTY) {
                N += (row + 1);
                goto find_empty;
            }
        }
    }
    find_empty:
    return N % 2 == 0;
};

void rotate_state(int state[SIDE][SIDE])
{
    for(int i = 0; i < SIDE / 2; i++) {
        for(int j = i; j < (SIDE-1)-i; j++) {
            int* tl = &(state[i][j]); //top-left
            int* bl = &(state[(SIDE-1)-i-(j-i)][i]); //bottom-left
            int* br = &(state[(SIDE-1)-i][(SIDE-1)-i-(j-i)]); //bottom-right
            int* tr = &(state[j][(SIDE-1)-i]); //top-right
            //swap
            int tmp = *tl;
            *tl = *bl;
            *bl = *br;
            *br = *tr;
            *tr = tmp;
        }
    }
};
