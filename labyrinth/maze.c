#include "maze.h"
#include <stdio.h>
#include "utils.h"

struct Maze {
  Vector *dimensions;
  Vector *start_position;
  Vector *end_position;
  Bitset *walls;
  uint64_t start_position_hash;
  uint64_t end_position_hash;
};

// Checks if position is correct and inside maze.
static bool is_position_valid(Maze *maze, Vector *position) {
  if (position == NULL ||
      vector_size(position) != vector_size(maze->dimensions)) {
    return false;
  }

  for (size_t i = 0; i < vector_size(position); i++) {
    uint64_t z_i = vector_get(position, i);
    if (z_i < 1 || z_i > vector_get(maze->dimensions, i)) {
      return false;
    }
  }

  return true;
}

// Checks if a position is not a wall.
static bool is_position_free(Maze *maze, size_t position_hash) {
  return !bitset_get(maze->walls, position_hash);
}

// Changes position to wall.
static void set_wall(Maze *maze, size_t position_hash) {
  bitset_set(maze->walls, position_hash);
}

// Returns hash of a position.
static size_t hash_position(Maze *maze, Vector *position) {
  size_t hash = 0, N = 1;
  for (size_t i = 0; i < vector_size(maze->dimensions); i++) {
    hash += (vector_get(position, i) - 1) * N;
    N *= vector_get(maze->dimensions, i);
  }

  return hash;
}

// Dehashes position and saves coordinates to passed vector.
static void dehash_position(Maze *maze, uint64_t hash, Vector *result) {
  vector_clear(result);
  for (size_t i = 0; i < vector_size(maze->dimensions); i++) {
    uint64_t n_i = vector_get(maze->dimensions, i);
    vector_push_back(result, 1 + hash % n_i);
    hash /= n_i;
  }
}

// Adds all possible to visit adjacent positions to passed vector.
static bool process_adjacent_positions(Maze *maze, Vector *position,
                                       size_t position_hash, Vector *next) {
  size_t N = 1;
  for (size_t i = 0; i < vector_size(maze->dimensions); i++) {
    for (size_t j = 0; j <= 1; j++) {
      // decrement (when j = 0) or increment (when j = 1) i-th coordinate
      size_t next_position_hash = position_hash - N * !j + N * j;
      size_t z_i = vector_get(position, i) - !j + j;

      // check if position shouldn't be processed
      if (z_i < 1 || z_i > vector_get(maze->dimensions, i) ||
          !is_position_free(maze, next_position_hash)) {
        continue;
      }

      // check if position is end position
      if (next_position_hash == maze->end_position_hash) {
        return true;
      }

      // process position
      vector_push_back(next, next_position_hash);
      set_wall(maze, next_position_hash);
    }

    N *= vector_get(maze->dimensions, i);
  }

  return false;
}

// Finds length of the shortest path from start to end position
// and returns it. If it doesn't exist, returns 0. Assumes that
// start and end positions are not equal.
static size_t find_shortest_path(Maze *maze) {
  size_t answer = 0, depth = 0;

  // will store hashes of positions
  Vector *current_depth_positions = vector_create();
  Vector *next_depth_positions = vector_create();

  // will store coordinates of current position
  Vector *position = vector_create();

  // initialize bfs using start position
  vector_push_back(current_depth_positions, maze->start_position_hash);
  set_wall(maze, maze->start_position_hash);

  while (!answer && !vector_is_empty(current_depth_positions)) {
    // current position
    uint64_t position_hash = vector_pop_back(current_depth_positions);
    dehash_position(maze, position_hash, position);

    // add adjacent positions to next_depth_positions
    if (process_adjacent_positions(maze, position, position_hash,
                                   next_depth_positions)) {
      // end position is adjacent to current position
      answer = depth + 1;
    }

    // check if all positions of current depth were visited
    if (vector_is_empty(current_depth_positions)) {
      ++depth;
      swap((void **)&current_depth_positions, (void **)&next_depth_positions);
    }
  }

  vector_free(current_depth_positions);
  vector_free(next_depth_positions);
  vector_free(position);

  return answer;
}

Maze *maze_create() {
  Maze *maze = (Maze *)safe_calloc(1, sizeof(Maze));
  return maze;
}

void maze_free(Maze *maze) {
  if (maze != NULL) {
    vector_free(maze->dimensions);
    vector_free(maze->start_position);
    vector_free(maze->end_position);
    bitset_free(maze->walls);
    free(maze);
  }
}

size_t maze_size(Maze *maze) {
  size_t size = 1;
  for (size_t i = 0; i < vector_size(maze->dimensions); i++) {
    size = safe_product(size, vector_get(maze->dimensions, i));
  }

  return size;
}

bool maze_set_dimensions(Maze *maze, Vector *dimensions) {
  maze->dimensions = dimensions;
  if (dimensions == NULL || vector_size(dimensions) == 0) {
    return false;
  }
  for (size_t i = 0; i < vector_size(dimensions); i++) {
    if (vector_get(dimensions, i) < 1 || vector_get(dimensions, i) > SIZE_MAX) {
      return false;
    }
  }

  return true;
}

bool maze_set_start_position(Maze *maze, Vector *position) {
  maze->start_position = position;
  if (is_position_valid(maze, position)) {
    maze->start_position_hash = hash_position(maze, position);
    return true;
  }

  return false;
}

bool maze_set_end_position(Maze *maze, Vector *position) {
  maze->end_position = position;
  if (is_position_valid(maze, position)) {
    maze->end_position_hash = hash_position(maze, position);
    return true;
  }

  return false;
}

bool maze_set_walls(Maze *maze, Bitset *walls) {
  maze->walls = walls;

  return walls != NULL;
}

bool maze_is_start_position_free(Maze *maze) {
  return is_position_free(maze, maze->start_position_hash);
}

bool maze_is_end_position_free(Maze *maze) {
  return is_position_free(maze, maze->end_position_hash);
}

void maze_solve(Maze *maze) {
  if (maze->start_position_hash == maze->end_position_hash) {
    printf("0\n");
    return;
  }

  size_t path_length = find_shortest_path(maze);
  if (path_length != 0) {
    printf("%zu\n", path_length);
  } else {
    printf("NO WAY\n");
  }
}
