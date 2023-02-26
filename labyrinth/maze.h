#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>
#include "bitset.h"
#include "vector.h"

typedef struct Maze Maze;

// Creates empty maze.
Maze *maze_create();

// Frees all allocated memory of passed maze.
void maze_free(Maze *maze);

// Returns maze size, i.e. product of its dimensions. If it
// exceeds SIZE_MAX, returns SIZE_MAX.
size_t maze_size(Maze *maze);

// Sets maze dimensions and checks if they're correct.
bool maze_set_dimensions(Maze *maze, Vector *dimensions);

// Sets maze start position and checks if it's correct.
bool maze_set_start_position(Maze *maze, Vector *position);

// Sets maze end position and checks if it's correct.
bool maze_set_end_position(Maze *maze, Vector *position);

// Sets maze walls and checks if they're correct.
bool maze_set_walls(Maze *maze, Bitset *walls);

// Checks if start position is free.
bool maze_is_start_position_free(Maze *maze);

// Checks if end position is free.
bool maze_is_end_position_free(Maze *maze);

// Prints length of the shortest path from start to end position or
// prints NO WAY if it doesn't exist.
void maze_solve(Maze *maze);

#endif  // MAZE_H
