#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Vector Vector;

// Creates empty vector.
Vector *vector_create();

// Creates vector from a string of non-negative numbers
// and returns it or returns NULL if the string is incorrect.
Vector *vector_create_from_string(char *str);

// Frees all allocated memory of passed vector.
void vector_free(Vector *v);

// Removes all elements but doesn't free allocated memory.
void vector_clear(Vector *v);

// Adds element to the end of vector.
void vector_push_back(Vector *v, uint64_t element);

// Removes the last element of vector and returns it.
uint64_t vector_pop_back(Vector *v);

// Returns i-th element of vector.
uint64_t vector_get(Vector *v, size_t i);

// Returns number of elements of vector.
size_t vector_size(Vector *v);

// Checks if vector is empty.
bool vector_is_empty(Vector *v);

#endif  // VECTOR_H
