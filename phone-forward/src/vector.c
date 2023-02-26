/** @file
 * Implementacja klasy przechowującej ciąg napisów jako tablicę.
 *
 * @date 2022
 */

#include "vector.h"
#include <stdlib.h>

/**
 * Struktura przechowująca ciąg napisów.
 */
struct Vector {
  char **elements;       ///< Tablica przechowująca elementy.
  size_t size;           ///< Pojemność tablicy @p elements.
  size_t elementsCount;  ///< Liczba elementów w tablicy @p elements.
};

Vector *vectorNew(void) {
  Vector *v = (Vector *)malloc(sizeof(Vector));
  if (v != NULL) {
    v->size = 1;
    v->elementsCount = 0;
    v->elements = (char **)malloc(v->size * sizeof(char *));
    if (v->elements == NULL) {
      free(v);
      v = NULL;
    }
  }

  return v;
}

void vectorDelete(Vector *v) {
  if (v != NULL) {
    for (size_t i = 0; i < v->elementsCount; i++) {
      free(v->elements[i]);
    }
    free(v->elements);
    free(v);
  }
}

bool vectorAdd(Vector *v, char *element) {
  if (v == NULL) {
    return false;
  }

  if (v->elementsCount == v->size) {
    // nie ma miejsca w tablicy, trzeba ją powiększyć
    char **newPtr = (char **)realloc(v->elements, 2 * v->size * sizeof(char *));
    if (newPtr == NULL) {
      return false;
    }
    v->size *= 2;
    v->elements = newPtr;
  }

  v->elements[v->elementsCount++] = element;

  return true;
}

char const *vectorGet(Vector const *v, size_t idx) {
  return v != NULL && idx < v->elementsCount ? v->elements[idx] : NULL;
}

void vectorRemoveLast(Vector *v) {
  free(v->elements[--v->elementsCount]);
}

void vectorSwap(Vector *v, size_t i, size_t j) {
  if (v != NULL && i < v->elementsCount && j < v->elementsCount && i != j) {
    char *temp = v->elements[i];
    v->elements[i] = v->elements[j];
    v->elements[j] = temp;
  }
}

size_t vectorSize(Vector const *v) {
  return v != NULL ? v->elementsCount : 0;
}

void vectorSort(Vector *v, int (*cmp)(void const *a, void const *b)) {
  qsort(v->elements, v->elementsCount, sizeof(char *), cmp);
}

void vectorShrink(Vector *v) {
  size_t newSize = v->elementsCount == 0 ? 1 : v->elementsCount;
  char **newPtr = (char **)realloc(v->elements, newSize * sizeof(char *));
  if (newPtr != NULL) {
    v->size = newSize;
    v->elements = newPtr;
  }
}
