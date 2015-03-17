#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

// Includes for colored version
#if defined _WIN64 || defined _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "main.h"

void displayWorld(World *world)
{
    int current_line = 0;

    for(int i=0; i < world->size * world->size; i++) {
        if (((int)i/world->size) != current_line)
            printf("\n");

        // World is populated by different kind of cells
        switch((world->cells+i)->state) {
            case NEW:
                #if defined _WIN64 || defined _WIN32
                SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                #else
                printf("\033[0;32m");
                #endif

                printf("0 ");
                break;
            case DEAD:
                #if defined _WIN64 || defined _WIN32
                SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
                #else
                printf("\033[0;31m");
                #endif

                printf("- ");
                break;
            case ALIVE:
                #if defined _WIN64 || defined _WIN32
                SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                #else
                printf("\033[0;34m");
                #endif

                printf("+ ");
                break;
            default:
                printf("? ");
                break;
        }

        current_line = (int)i/world->size;
    }
    printf("\n\n");
}

void newGeneration(World *world)
{
    if(step == 0) {
        // Then He created dead and new cells
        srand(time(NULL));

        for(int i=0; i < world->size * world->size; i++) {
            (world->cells+i)->state = (int)(rand() % 2)*2; // They was NEW (0*2) or DEAD (1*2)

            // God doesn't like mess, so each cell has
            (world->cells+i)->x = i%world->size; // a specific column
            (world->cells+i)->y = (int)i/world->size; // and a specific line
        }
    }
    else {
        // Now, let the nature decide if cell must to die or live
        for(int i=0; i < world->size * world->size; i++) {

            Cell *current_cell = (world->cells+i);
            int pop = getAliveNeighbors(world, current_cell);

            if( pop < 2 && (current_cell->state == ALIVE || current_cell->state == NEW) ) { // Under-population
                current_cell->state = DEAD;
            }
            else if( (pop == 2 || pop == 3) && current_cell->state == NEW ) { // Cell growth, no more a baby
                current_cell->state = ALIVE;
            }
            else if( pop > 3 && (current_cell->state == ALIVE || current_cell->state == NEW) ) { // Overcrowding
                current_cell->state = DEAD;
            }
            else if (pop == 3 && current_cell->state == DEAD) { // Cell resurrection
                current_cell->state = NEW;
            }

        }
    }
}

int getAliveNeighbors(World *world, Cell *cell)
{
    int count = 0;

    for(int a = -1; a <= 1; a++) { // x axis movements
        for(int b = -1; b <= 1; b++) { // y axis movements
            if( !(a == 0 && b == 0) ) { // Exclude the current cell

                int test_index = ((cell->y+b) * world->size) + (cell->x+a);

                if( test_index >= 0 && test_index < world->size*world->size ) {

                    Cell *test_cell = (world->cells + test_index );
                    if( test_cell->x+test_cell->y >= cell->x-1+cell->y-1 && test_cell->x+test_cell->y <= cell->x+1+cell->y+1 ) {
                        if( test_cell->state == ALIVE || test_cell->state == NEW )
                            count++;
                    }
                }
            }
        }
    }
    if (count >= 0 && count <=  8)
        return count;
    else
        return 0;

}

void showTitle()
{
    #if defined _WIN64 || defined _WIN32
    SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
    #else
    printf("\033[0;37m");
    #endif

    printf("=================================\n");
    printf("===== CONWAY'S GAME OF LIFE =====\n");
    printf("=================================\n\n");
}

void clearScreen()
{

    #if defined _WIN64 || defined _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void forceKill(){
    clearScreen();
    printf("\n\nYou've just destroyed the world. But the Good always win, don't forget it.\n\n");
    exit(0);
}

int main(void)
{

    // In the beginning God created empty limited world
    World *world = malloc(sizeof(World));
    world->size = 10;
    world->cells = malloc(world->size * world->size * sizeof(Cell));

    #if defined _WIN64 || defined _WIN32
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    #endif

    signal(SIGINT, forceKill); // User plans to kill the life cycle of Conway :(
    clearScreen();

    while(armageddon != 1) {
        showTitle();

        printf("\nGeneration #%d \n\n", step+1);
        newGeneration(world);
        displayWorld(world);

        if(automatic != 1) {
            char dictator_answer = NULL;

            #if defined _WIN64 || defined _WIN32
            SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
            #else
            printf("\033[0;37m");
            #endif

            printf("\nContinue the generation: yes, no, auto ? (y/n/a) [y] ");
            scanf("%c", &dictator_answer);

            if(dictator_answer == 'n' || dictator_answer == 'N') {
                armageddon = 1;
            }
            else if(dictator_answer == 'a' || dictator_answer == 'A') {
                step++;
                fflush(stdin);
                automatic = 1;
            }
            else {
                step++;
                fflush(stdin);
            }
        }
        else {
            #if defined _WIN64 || defined _WIN32
            SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
            #else
            printf("\033[0;37m");
            #endif

            printf("\n<Control-C> for stopping \n\n");
            step++;
            sleep(1);
        }

    clearScreen();

    }

    printf("\n\nYou've just destroyed the world. But the Good always win, don't forget it.\n\n");

    return 0;
}
