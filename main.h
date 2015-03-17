#ifndef MAIN_H
#define MAIN_H

typedef enum stateType {
    NEW   = 0,
    ALIVE = 1,
    DEAD  = 2
} stateType;

typedef struct Cell
{
    int x;
    int y;
    stateType state;
} Cell;

typedef struct World
{
    unsigned int size;
    Cell *cells;

} World;

void displayWorld(World *world);
void newGeneration(World *world);
int getAliveNeighbors(World *world, Cell *cell);
void showTitle();
void clearScreen();

#if defined _WIN64 || defined _WIN32
HANDLE console;
#endif

int armageddon = 0;
int step = 0;
int automatic = 0;


#endif // MAIN_H
