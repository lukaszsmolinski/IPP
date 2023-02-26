#ifndef INPUT_H
#define INPUT_H

#include "maze.h"

// Reads standard input and saves it to passed maze. If input is incorrect,
// prints error and returns false. Otherwise, returns true.
bool read_maze_data(Maze *maze);

#endif  // INPUT_H
