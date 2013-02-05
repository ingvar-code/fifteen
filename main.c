/*
 * date: 18.01.2011
 * author: ingvar
 * 
 * Main file
 * 
 */
#include "fifteen.h"
#include <time.h>

static short int find(int[SIDE][SIDE]);
static short int parse(char*, int[SIDE][SIDE]);

int main(int argc, char** argv)
{
    short int is_solved_state(int[SIDE][SIDE]);
    void show_state(int [SIDE][SIDE], int turn);
    void rotate_state(int[SIDE][SIDE]);
    
    char* filename = (argc > 1) ? *(argv+1) : "data.txt";
    int state[SIDE][SIDE];
    if(!parse(filename, state)) {
        return 1;
    }
    
    int ask = 0;
    for(int i = 0; i < 4; i++) {
        printf("\nTry to solve state:\n");
        show_state(state, 0);
        for(int j = 0; j < SIDE; j++) {
            putchar('_');
            putchar('_');
            putchar('_');
        }
        putchar('\n');
        if(is_solved_state(state)) {
            if(find(state)) {
                return 0;
            }
        }
        else {
            printf("This state is unsolved!\n");
            if(!ask) {
                ask = 1;
                printf("But if rotate this board, we can try to solve it. Do you want to try?(Y/N): ");
                int ans = getchar();
                if(ans == 'N' || ans == 'n') {
                    return 0;
                }
            }
        }
        rotate_state(state);
    }
    printf("Rotate does not work! :( Combination unsolved!");
    return 0;
};

static short find(int state[SIDE][SIDE]) {
    struct STAT* astar(int[SIDE][SIDE]); //use astar search
    void show_steps(struct STEP*);
    void show_state(int [SIDE][SIDE], int);
    
    struct STAT* stat = astar(state);
    time_t sec = (time_t) stat->seconds;
    struct tm* time = gmtime(&sec);
    if(stat->solve == NULL || stat->solve->rate > 0) {
        printf("Solve not found! Explored states: %d. Elapsed time: %02d:%02d:%02d. Speed: %d states/sec.\n",
                stat->explored, time->tm_hour, time->tm_min, time->tm_sec, stat->speed);
        if(stat->solve != NULL) {
            printf("Better state. Turn %d:\n", stat->solve->count);
            show_state(stat->solve->state, 0);
        }
        return 0;
    }
    printf("Solve found on turn: %d! Explored states: %d. Elapsed time: %02d:%02d:%02d. Speed: %d states/sec.\n",
            stat->solve->count, stat->explored, time->tm_hour, time->tm_min, time->tm_sec, stat->speed);
    return 1;
};

static short int parse(char* filename, int state[SIDE][SIDE])
{
    char* err_mes;
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        printf("File \'%s\' not found!", filename);
        return 0;
    }
    
    int NUMS = SIDE * SIDE;
    int num_set[NUMS];
    for(int i = 0; i < NUMS; i++) {
        num_set[i] = 0;
    }
    
    for(int row = 0; row < SIDE; row++) {
        for(int col = 0; col < SIDE; col++) {
            int n;
            if(fscanf(file, "%d", &n) == EOF) {
                err_mes = "File not contains all need numbers!";
                goto error;
            }
            if(n < 0 || n >= NUMS) {
                err_mes = "File contains incorrect numbers!";
                goto error;
            }
            if(num_set[n]) {
                err_mes = "File contains duplicate numbers!";
                goto error;
            }
            state[row][col] = n;
            num_set[n] = 1;
        }
    }
    for(int i = 0; i < NUMS; i++) {
        if(!num_set[i]) {
            err_mes = "File not contains all need numbers!";
            goto error;
        }
    }
    fclose(file);
    return 1;
    error:
    fclose(file);
    printf("file parse: %s", err_mes);
    return 0;
};
