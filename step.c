/*
 * date: 18.01.2011
 * author: ingvar
 * 
 * Step support functions
 * 
 */
#include "fifteen.h"

struct STEP* create_step(struct STEP* parent)
{
    void copy_states(int[SIDE][SIDE], int[SIDE][SIDE]);
    
    struct STEP* next = (struct STEP *) malloc(sizeof(struct STEP));
    if(parent == NULL) {
        next->prev = NULL;
        next->count= 0;
        next->rate = 0;
        next->turn = NONE;
        for(int i = 0; i < SIDE; i++) {
            for(int j = 0; j < SIDE; j++) {
                next->state[i][j] = EMPTY;
            }
        }
    }
    else {
        next->prev = parent;
        next->count= parent->count + 1;
        next->rate = 0;
        next->turn = NONE;
        copy_states(parent->state, next->state);
    }
    
    return next;
};

short int compare_steps(struct STEP* first, struct STEP* second)
{
    if(first->rate <= 0 && second->rate > 0) {
        return -1;
    }
    else if(first->rate > 0 && second->rate <= 0) {
        return 1;
    }
    else {
        int fr = first->rate + first->count;
        int sr = second->rate + second->count;
        return fr - sr;
    }
};

struct STEP* remove_min(struct STEP* array[], int* length)
{
    int pos;
    struct STEP* min = NULL;
    for(int i = 0; i < *length; i++) {
        if(min == NULL) {
            min = array[i];
        }
        else {
            int comp = compare_steps(array[i], min);
            if(comp < 0 || (comp == 0 && array[i]->count < min->count)) {
                min = array[i];
                pos = i;
            }
        }
    }
    (*length)--;
    for(int i = pos; i < *length; i++) {
        array[i] = array[i+1];
    }
    return min;
};

void show_steps(struct STEP* step)
{
    void show_state(int[SIDE][SIDE], enum TURN turn);
    
    enum TURN turn = step->turn;
    if(step->prev != NULL) {
        show_steps(step->prev);
    }
    printf("turn: %d rate: %d\n___________\n", step->count, step->rate);
    show_state(step->state, turn);
    putchar('\n');
    putchar('\n');
};
