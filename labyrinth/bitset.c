#include "bitset.h"
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

// Bits stored in one element of bitset
#define BITS 64

struct Bitset {
  uint64_t *data;
  size_t size;
};

// Checks if a generator is correct.
static bool is_correct_generator(Vector *gen) {
  if (gen == NULL || vector_size(gen) != 5 || vector_get(gen, 2) == 0) {
    return false;
  }
  for (size_t i = 0; i <= 4; i++) {
    if (vector_get(gen, i) > UINT32_MAX) {
      return false;
    }
  }

  return true;
}

// Returns hexadecimal number size if it's correct or 0 otherwise.
static size_t is_correct_hexadecimal(char *hex) {
  if (hex[0] != '0' || hex[1] != 'x' || hex[2] == '\0') {
    return 0;
  }

  size_t hex_size = 2;
  while (hex[hex_size] != '\0') {
    if (!isxdigit(hex[hex_size++])) {
      return 0;
    }
  }

  return hex_size;
}

// Creates bitset from a hexadecimal number represented as string.
// If it's incorrect or there exists bit >= bitset_size, returns NULL.
static Bitset *bitset_create_from_hexadecimal(char *hex, size_t bitset_size) {
  size_t hex_size = is_correct_hexadecimal(hex);
  if (hex_size == 0) {
    // incorrect hexadecimal number
    return NULL;
  }

  Bitset *bitset = bitset_create(bitset_size);
  size_t bit_number = 0;
  for (size_t i = hex_size - 1; i >= 2; i--) {
    int digit = (int)strtoul(&hex[i], NULL, 16);
    hex[i] = '\0';

    for (uint32_t j = 0; j <= 3; j++) {
      if ((digit & (1 << j)) && !bitset_set(bitset, bit_number)) {
        // tried to set bit over bitset size
        bitset_free(bitset);
        return NULL;
      }

      ++bit_number;
    }
  }

  return bitset;
}

// Creates bitset from a generator. If it's incorrect, returns NULL.
static Bitset *bitset_create_from_generator(Vector *gen, size_t bitset_size) {
  if (!is_correct_generator(gen)) {
    return NULL;
  }

  Bitset *bitset = bitset_create(bitset_size);
  uint64_t a = vector_get(gen, 0), b = vector_get(gen, 1),
           m = vector_get(gen, 2), r = vector_get(gen, 3),
           s = vector_get(gen, 4);

  for (uint32_t i = 0; i < r; i++) {
    s = (a * s + b) % m;
    size_t w_i = s % bitset_size;

    if (!bitset_get(bitset, w_i)) {
      // set all bits that satisfy  bit mod 2^32 = w_i
      while (w_i < bitset_size) {
        bitset_set(bitset, w_i);
        w_i = safe_sum(w_i, 1ULL << 32);
      }
    }
  }

  return bitset;
}

Bitset *bitset_create(size_t size) {
  Bitset *bitset = (Bitset *)safe_malloc(sizeof(Bitset));
  bitset->data = (uint64_t *)safe_calloc(1 + size / 64, sizeof(uint64_t));
  bitset->size = size;

  return bitset;
}

Bitset *bitset_create_from_string(char *str, size_t size) {
  Bitset *walls = NULL;
  if (str[0] == '0') {
    walls = bitset_create_from_hexadecimal(str, size);
  } else if (str[0] == 'R') {
    Vector *gen = vector_create_from_string(&str[1]);
    walls = bitset_create_from_generator(gen, size);
    vector_free(gen);
  }

  return walls;
}

void bitset_free(Bitset *bitset) {
  if (bitset != NULL) {
    free(bitset->data);
    free(bitset);
  }
}

bool bitset_set(Bitset *bitset, size_t i) {
  if (i >= bitset->size) {
    return false;
  }

  uint64_t n = i / BITS;
  uint64_t m = i - n;
  bitset->data[n] |= (1ULL << m);

  return true;
}

bool bitset_get(Bitset *bitset, size_t i) {
  uint64_t n = i / BITS;
  uint64_t m = i - n;

  return (bitset->data[n] & (1ULL << m)) > 0;
}
