#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Returns min{a + b, SIZE_MAX}
static inline size_t safe_sum(size_t a, size_t b) {
  return a <= SIZE_MAX - b ? a + b : SIZE_MAX;
}

// Returns min{a * b, SIZE_MAX}
static inline size_t safe_product(size_t a, size_t b) {
  return (b > 0 && a > SIZE_MAX / b) ? SIZE_MAX : a * b;
}

// Swaps two pointers.
static inline void swap(void **ptr1, void **ptr2) {
  void *temp = *ptr2;
  *ptr2 = *ptr1;
  *ptr1 = temp;
}

// Prints error message to standard error.
static inline void print_error(int error_code) {
  fprintf(stderr, "ERROR %d\n", error_code);
}

// Prints error message to standard error and exits program with code 1.
static inline void error(int error_code) {
  print_error(error_code);
  exit(1);
}

// Tries to malloc. On failure, calls error with code 0.
static inline void *safe_malloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    error(0);
  }

  return ptr;
}

// Tries to calloc. On failure, calls error with code 0.
static inline void *safe_calloc(size_t num, size_t size) {
  void *ptr = calloc(num, size);
  if (ptr == NULL) {
    error(0);
  }

  return ptr;
}

// Tries to realloc. On failure, calls error with code 0.
static inline void *safe_realloc(void *ptr, size_t new_size) {
  ptr = realloc(ptr, new_size);
  if (ptr == NULL) {
    error(0);
  }

  return ptr;
}

#endif  // UTILS_H
