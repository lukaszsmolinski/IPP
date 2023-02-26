#include "input.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include "bitset.h"
#include "utils.h"
#include "vector.h"

// Reads standard input until EOF or \n occurs and saves it to passed array.
// Leading and trailing whitespaces are skipped and array ends with \0.
// If the first character is EOF, returns false. Otherwise, returns true.
static bool read_line(char **line, size_t *line_size) {
  size_t line_length = 0;
  (*line)[0] = '\0';

  int c = getchar();
  if (c == EOF) {
    return false;
  }

  // skip leading whitespaces
  while (isspace(c) && c != '\n') {
    c = getchar();
  }

  while (c != '\n' && c != EOF) {
    (*line)[line_length++] = (char)c;
    c = getchar();

    if (*line_size == line_length) {
      *line_size *= 2;
      *line = (char *)safe_realloc(*line, *line_size * sizeof(char));
    }
  }

  // skip trailing whitespaces
  while (line_length > 0 && isspace((*line)[line_length - 1])) {
    line_length--;
  }

  (*line)[line_length] = '\0';

  return true;
}

// Processes 4 lines of input. If it's incorrect, returns first
// incorrect line number. Otherwise, returns 0.
static int process_input(Maze *maze, char **line, size_t *line_size) {
  read_line(line, line_size);
  if (!maze_set_dimensions(maze, vector_create_from_string(*line))) {
    return 1;
  }

  read_line(line, line_size);
  if (!maze_set_start_position(maze, vector_create_from_string(*line))) {
    return 2;
  }

  read_line(line, line_size);
  if (!maze_set_end_position(maze, vector_create_from_string(*line))) {
    return 3;
  }

  read_line(line, line_size);
  if (!maze_set_walls(maze,
                      bitset_create_from_string(*line, maze_size(maze)))) {
    return 4;
  }

  return 0;
}

bool read_maze_data(Maze *maze) {
  size_t line_size = 1;
  char *line = (char *)safe_malloc(sizeof(char));

  // err is number of the first incorrect line or 0 if they are correct
  int err = process_input(maze, &line, &line_size);

  if (!err) {
    if (!maze_is_start_position_free(maze)) {
      err = 2;
    } else if (!maze_is_end_position_free(maze)) {
      err = 3;
    } else if (read_line(&line, &line_size)) {
      // correct input is only 4 lines long
      err = 5;
    }
  }

  free(line);

  if (err) {
    print_error(err);
  }

  return !err;
}
