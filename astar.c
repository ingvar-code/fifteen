/*
 * date: 18.01.2011
 * author: ingvar
 * 
 * Astar search functions
 * 
 */
#include "fifteen.h"
#include <time.h>

/* GLOBAL VARIABLES */
#define MAX_SIZE 50000
static struct STEP* explored[MAX_SIZE];
static int lenghtE = 0;
static struct STEP* frontier[MAX_SIZE];
static int lenghtF = 0;
/* FUNCTIONS */
struct STEP* create_step(struct STEP*);
struct STEP* remove_min(struct STEP*[], int*);
struct CELL* get_empty(int[SIDE][SIDE]);
void copy_states(int[SIDE][SIDE], int[SIDE][SIDE]);
short int compare_steps(struct STEP*, struct STEP*);
short int equals_states(int[SIDE][SIDE], int[SIDE][SIDE]);
static void rate(struct STEP*);
static void explore(struct STEP*);
static void clear(void);

struct STAT* astar(int state[SIDE][SIDE])
{
    printf("in progress...\r");
    clear();
    time_t s, e;
    struct STEP* first;
    struct STEP* next;
    time(&s);
    first = create_step(NULL);
    copy_states(state, first->state);
    rate(first);
    frontier[lenghtF++] = first;
    
    while(lenghtF > 0) {
        next = remove_min(frontier, &lenghtF);
        if(next->rate <= 0) {
            break;
        }
        else if(lenghtF >= MAX_SIZE-4 || lenghtE >= MAX_SIZE) {
            for(int i = 0; i < lenghtE; i++) {
                if(compare_steps(explored[i], next) < 0) {
                    next = explored[i];
                }
            }
            break;
        }
        else {
            explore(next);
        }
    }
    time(&e);
    struct STAT* stat = (struct STAT*) malloc(sizeof(struct STAT));
    stat->solve = next;
    stat->explored = lenghtE;
    stat->seconds = (int) (e - s);
    stat->speed = MAX(lenghtE, 1) / MAX(stat->seconds, 1);
    return stat;
};

static void rate(struct STEP* step)
{
    int moves_count(int, int, int);
    
    int pv = 0; //position value
    int mv = 0; //move count value
    for(int row = 0; row < SIDE; row++) {
        for(int col = 0; col < SIDE; col++) {
            int num = step->state[row][col];
            if(num != EMPTY && num != correct_num(row, col)) {
                pv += (SIDE - MIN(row, col));
                mv += moves_count(num, row, col) * (SIDE - MIN(row, col));
            }
        }
    }
    step->rate = pv + mv;
};

static void explore(struct STEP* step)
{
    explored[lenghtE++] = step;
    struct CELL* empty = get_empty(step->state);
    struct CELL* taken = (struct CELL*) malloc(sizeof(struct CELL));
    
    for(enum TURN turn = UP; turn <= LEFT; turn++) {
        taken->row = 0;
        taken->col = 0;
        switch (turn) {
            case UP:
                taken->row = empty->row + 1;
                taken->col = empty->col;
                break;
            case RIGHT:
                taken->row = empty->row;
                taken->col = empty->col - 1;
                break;
            case DOWN:
                taken->row = empty->row - 1;
                taken->col = empty->col;
                break;
            case LEFT:
                taken->row = empty->row;
                taken->col = empty->col + 1;
                break;
        }
        if(taken->row < 0 || taken->row >= SIDE || taken->col < 0 || taken->col >= SIDE) {
            continue;
        }
        
        struct STEP* next = create_step(step);
        next->state[empty->row][empty->col] = next->state[taken->row][taken->col];
        next->state[taken->row][taken->col] = EMPTY;
        int equals = 0;
        for(int i = 0; i < lenghtF; i++) {
            if(equals_states(frontier[i]->state, next->state)) {
                equals = 1;
                break;
            }
        }
        for(int i = 0; i < lenghtE; i++) {
            if(equals_states(explored[i]->state, next->state)) {
                equals = 1;
                break;
            }
        }
        if(equals) {
            free(next);
            continue;
        }
        next->turn = turn;
        rate(next);
        frontier[lenghtF++] = next;
    }
    
    free(empty);
    free(taken);
};

static void clear(void) {
    if(lenghtF > 0) {
        for(int i = 0; i < lenghtF; i++) {
            free(frontier[i]);
        }
        lenghtF = 0;
    }
    if(lenghtE > 0) {
        for(int i = 0; i < lenghtE; i++) {
            free(explored[i]);
        }
        lenghtE = 0;
    }
}
