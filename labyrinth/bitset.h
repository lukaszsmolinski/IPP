#ifndef BITSET_H
#define BITSET_H

#include <stdbool.h>
#include <stddef.h>
#include "vector.h"

typedef struct Bitset Bitset;

// Creates empty (filled with zeros) bitset with given size.
Bitset* bitset_create(size_t size);

// Creates bitset from a string, which represents either a hexadecimal
// number or a generator, and returns it. If it's incorrect or it tries
// to set bit >= size, returns NULL.
Bitset* bitset_create_from_string(char* str, size_t size);

// Frees all allocated memory of passed bitset.
void bitset_free(Bitset* bitset);

// If i < bitset size, sets i-th bit of bitset and returns true.
// Otherwise, returns false.
bool bitset_set(Bitset* bitset, size_t i);

// Checks if i-th bit is set.
bool bitset_get(Bitset* bitset, size_t i);

#endif  // BITSET_H
