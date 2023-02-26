#include "vector.h"
#include <errno.h>
#include "utils.h"

struct Vector {
  uint64_t *data;
  size_t size;
  size_t elements_number;
};

Vector *vector_create() {
  Vector *v = (Vector *)safe_malloc(sizeof(Vector));
  v->size = 1;
  v->data = (uint64_t *)safe_malloc(v->size * sizeof(uint64_t));
  v->elements_number = 0;

  return v;
}

Vector *vector_create_from_string(char *str) {
  Vector *v = vector_create();
  char *start = str, *end = NULL;
  while (*start != '\0') {
    errno = 0;
    uint64_t number = strtoull(start, &end, 10);

    // check if number is out of range or invalid character occured
    if (errno == ERANGE || start == end) {
      vector_free(v);
      return NULL;
    }

    start = end;
    vector_push_back(v, number);
  }

  return v;
}

void vector_free(Vector *v) {
  if (v != NULL) {
    free(v->data);
    free(v);
  }
}

void vector_clear(Vector *v) {
  v->elements_number = 0;
}

void vector_push_back(Vector *v, uint64_t element) {
  // check if vector needs more space
  if (v->elements_number == v->size) {
    v->size *= 2;
    v->data = (uint64_t *)safe_realloc(v->data, v->size * sizeof(uint64_t));
  }

  v->data[v->elements_number++] = element;
}

uint64_t vector_pop_back(Vector *v) {
  return v->data[--v->elements_number];
}

uint64_t vector_get(Vector *v, size_t i) {
  return v->data[i];
}

size_t vector_size(Vector *v) {
  return v->elements_number;
}

bool vector_is_empty(Vector *v) {
  return v->elements_number == 0;
}
