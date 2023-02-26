/** @file
 * Implementacja klasy przechowującej ciąg napisów jako cykliczną listę.
 *
 * @date 2022
 */

#include "list.h"
#include <stdlib.h>

/**
 * Struktura przechowująca ciąg napisów.
 */
struct List {
  char *value;     ///< Przechowywany napis lub NULL dla początkowego elementu.
  List *next;      ///< Następny element listy.
  List *previous;  ///< Poprzedni element listy.
};

/**
 * Struktura przechowująca iterator listy.
 */
struct ListIterator {
  List const *start;    ///< Początkowy element iteratora.
  List const *current;  ///< Obecny element.
};

List *listNew(void) {
  List *list = (List *)calloc(1, sizeof(List));
  if (list != NULL) {
    list->next = list->previous = list;
  }
  return list;
}

void listDelete(List *list) {
  if (list != NULL) {
    List *current = list;
    do {
      // usuwanie danych w current
      List *next = current->next;
      free(current->value);
      free(current);
      current = next;
    } while (current != list);
  }
}

List *listAdd(List *list, char *value) {
  if (list == NULL) {
    return NULL;
  }

  // mid to nowy element listy, a right i left to elementy po prawej i po lewej
  List *left = list, *mid = listNew(), *right = list->next;
  if (mid == NULL) {
    return NULL;
  }

  // wstawianie mid pomiędzy left i right
  mid->value = value;
  mid->previous = left;
  mid->next = right;
  left->next = mid;
  right->previous = mid;

  return mid;
}

char *listGet(List const *list) {
  if (listEmpty(list)) {
    return NULL;
  }
  return list->value == NULL ? list->next->value : list->value;
}

bool listEmpty(List const *list) {
  return list == NULL || list->next == list;
}

void listRemoveFirst(List *list) {
  if (list != NULL && !listEmpty(list)) {
    if (list->value == NULL) {
      // jesteśmy w elemencie początkowym, którego nie można usunąć
      list = list->next;
    }

    // zmiana odpowiednich pól elementów sąsiednich i usuwanie elementu
    list->previous->next = list->next;
    list->next->previous = list->previous;
    free(list->value);
    free(list);
  }
}

ListIterator *listIteratorNew(List const *list) {
  if (listEmpty(list)) {
    return NULL;
  }

  ListIterator *iter = (ListIterator *)malloc(sizeof(ListIterator));
  if (iter == NULL) {
    return NULL;
  }

  // nie chcemy, żeby iterator wskazywał na element początkowy
  iter->start = iter->current = (list->value == NULL ? list->next : list);

  return iter;
}

bool listIteratorNext(ListIterator *listIterator) {
  listIterator->current = listIterator->current->next;
  if (listIterator->current->value == NULL) {
    // pomijamy element początkowy
    listIterator->current = listIterator->current->next;
  }
  return listIterator->current != listIterator->start;
}

char const *listIteratorValue(ListIterator const *listIterator) {
  return listIterator->current->value;
}

void listIteratorDelete(ListIterator *listIterator) {
  free(listIterator);
}
