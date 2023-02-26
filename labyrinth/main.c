#include "input.h"
#include "maze.h"

int main() {
  Maze *maze = maze_create();

  if (read_maze_data(maze)) {
    maze_solve(maze);
  }

  maze_free(maze);

  return 0;
}
