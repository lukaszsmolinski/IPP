/** @file
 * Implementacja klasy definiującej drzewo trie.
 *
 * @date 2022
 */

#include "trie.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "list.h"
#include "string_utils.h"

/**
 * Rozmiar alfabetu kluczy i wartości.
 */
#define ALPHABET_SIZE 12

/**
 * Wartość w korzeniu.
 */
#define ROOT_VALUE "!"

/**
 * Struktura reprezentująca wierzchołek drzewa trie.
 */
struct Trie {
  char *value;  ///< Wartość w wierzchołku.
  List *keys;  ///< Lista kluczy wierzchołków, których wartość jest kluczem
               ///< obecnego wierzchołka. Jeśli jest pusta, ma wartość NULL.
  List *keysInRev;  ///< Lista @p keys wierzchołka o kluczu @p value.
                    ///< Pierwszym elementem tej listy jest klucz obecnego
                    ///< wierzchołka.
  Trie *revNode;    ///< Wierzchołek o kluczu @p value.
  Trie *previous;   ///< Poprzedni wierzchołek.
  Trie *next[ALPHABET_SIZE];  ///< Następne wierzchołki.
  uint8_t nextCount;  ///< Liczba różnych od NULL elementów tablicy @p next.
  uint8_t order;  ///< Liczba wskazująca, którym dzieckiem jest wierzchołek.
};

/** @brief Dodaje wierzchołek do drzewa trie.
 * @param[in,out] trie – wskaźnik na wierzchołek, do którego dodajemy dziecko;
 * @param[in] n – liczba wskazująca, którym dzieckiem będzie dodany wierzchołek.
 * @return Wartość @p true, gdy wierzchołek został dodany lub istniał wcześniej.
 *         Wartość @p false, gdy nie udało się alokować pamięci.
 */
static bool trieAddNode(Trie *trie, int n) {
  if (trie->next[n] != NULL) {
    return true;
  }

  if ((trie->next[n] = (Trie *)calloc(1, sizeof(Trie))) == NULL) {
    return false;
  }

  trie->nextCount++;
  trie->next[n]->order = n;
  trie->next[n]->previous = trie;

  return true;
}

/** @brief Usuwa niepotrzebny wierzchołek i jego niepotrzebnych przodków.
 * Niepotrzebne wierzchołki nie zawierają żadnej wartości, ich lista @p keys
 * jest pusta lub ma wartość NULL, nie mają dzieci i nie są korzeniem.
 * Nic nie robi jeśli @p trie ma wartość NULL.
 * @param[in,out] trie – wskaźnik na wierzchołek;
 */
static void trieDeleteUnusedBranch(Trie *trie) {
  while (trie != NULL && trie->nextCount == 0 && trie->value == NULL &&
         listEmpty(trie->keys)) {
    Trie *prev = trie->previous;

    if (prev != NULL) {
      prev->nextCount--;
      prev->next[trie->order] = NULL;
    }
    free(trie->value);
    listDelete(trie->keys);
    free(trie);

    trie = prev;
  }
}

/** @brief Znajduje wierzchołek powiązany z danym kluczem.
 * Jeśli taki wierzchołek nie istnieje, tworzy go. Jeśli nie uda się
 * alokować pamięci, drzewo pozostaje niezmienione.
 * @param[in,out] trie – wskaźnik na wierzchołek będący korzeniem drzewa;
 * @param[in] key – klucz szukanego wierzchołka.
 * @return Wskaźnik na szukany wierzchołek lub NULL jeśli nie udało
 *         się alokować pamięci.
 */
static Trie *trieGetNode(Trie *trie, char const *key) {
  while (*key != '\0') {
    uint8_t dir = strToInt(key++);
    if (trie->next[dir] == NULL) {
      if (!trieAddNode(trie, dir)) {
        // nie udało się alokować pamięci
        trieDeleteUnusedBranch(trie);
        return NULL;
      }
    }
    trie = trie->next[dir];
  }
  return trie;
}

/** @brief Usuwa wartość w wierzchołku.
 * Oprócz wartości, usuwane są również powiązane z nią dane przechowywane
 * w wierzchołku odwrotnym (czyli takim, którego kluczem jest usuwana
 * wartość). Usuwane są niepotrzebne wierzchołki (czyli takie, jakie opisano
 * w dokumentacji funkcji @ref trieDeleteUnusedBranch).
 * @param[in,out] trie – wskaźnik na wierzchołek.
 */
static void trieRemoveNodeValue(Trie *trie) {
  // usuwanie danych w wierzchołku odwrotnym
  listRemoveFirst(trie->keysInRev);
  if (trie->revNode != NULL && listEmpty(trie->revNode->keys)) {
    listDelete(trie->revNode->keys);
    trie->revNode->keys = NULL;
  }
  trieDeleteUnusedBranch(trie->revNode);

  // usuwanie danych w wierzchołku
  free(trie->value);
  trie->keysInRev = NULL;
  trie->value = NULL;
  trie->revNode = NULL;
  trieDeleteUnusedBranch(trie);
}

/** @brief Usuwa wartość w wierzchołku i wartości w jego poddrzewie.
 * Dla każdego wierzchołka wywoływana jest funkcja @ref trieRemoveNodeValue.
 * @param[in,out] trie – wskaźnik na wierzchołek.
 */
static void trieRemoveValues(Trie *trie) {
  if (trie == NULL) {
    return;
  }

  uint8_t i = 0;
  Trie *end = trie->previous;
  while (trie != end) {
    if (trie->value == NULL) {
      // wierzchołek staje się korzeniem, żeby nie został usunięty
      // w trakcie przetwarzania potomków
      trie->value = ROOT_VALUE;
    }

    if (i == ALPHABET_SIZE) {
      // całe poddrzewo zostało już przetworzone, więc można wracać do rodzica
      if (strcmp(trie->value, ROOT_VALUE) == 0) {
        trie->value = NULL;
      }
      i = trie->order + 1;
      Trie *prev = trie->previous;
      trieRemoveNodeValue(trie);
      trie = prev;
    } else if (trie->next[i] == NULL) {
      // dziecko numer i nie istnieje, więc je pomijamy
      ++i;
    } else {
      // dziecko numer i istnieje, więc można do niego przejść
      trie = trie->next[i];
      i = 0;
    }
  }
}

Trie *trieNew(void) {
  Trie *trie = (Trie *)calloc(1, sizeof(Trie));

  if (trie != NULL) {
    trie->value = ROOT_VALUE;
  }
  return trie;
}

void trieDelete(Trie *trie) {
  if (trie != NULL) {
    trieRemoveValues(trie);
  }
}

bool trieInsert(Trie *trie, char *key, char *val) {
  // wierzchołki powiązane z kluczem i wartością
  Trie *trieKey = NULL, *trieValue = NULL;
  List *list;

  // wszystkie operacje, które mogą się nie udać - tworzenie dwóch
  // wierzchołków oraz dodawanie klucza do wierzchołka powiązanego z val
  if ((trieKey = trieGetNode(trie, key)) == NULL ||
      (trieValue = trieGetNode(trie, val)) == NULL ||
      (trieValue->keys == NULL && (trieValue->keys = listNew()) == NULL) ||
      (list = listAdd(trieValue->keys, key)) == NULL) {
    trieDeleteUnusedBranch(trieKey);
    trieDeleteUnusedBranch(trieValue);
    return false;
  }

  // zastąpienie poprzedniej wartości
  free(trieKey->value);
  trieKey->value = val;

  // usunięcie połączenia z obecnym odwrotnym wierzchołkiem
  listRemoveFirst(trieKey->keysInRev);
  trieKey->keysInRev = list;
  if (trieKey->revNode != NULL && listEmpty(trieKey->revNode->keys)) {
    listDelete(trieKey->revNode->keys);
    trieKey->revNode->keys = NULL;
    trieDeleteUnusedBranch(trieKey->revNode);
  }

  trieKey->revNode = trieValue;

  return true;
}

char *trieGet(Trie const *trie, char const *key, size_t *n) {
  char *result = NULL;
  size_t currentDepth = 0;
  *n = 0;
  while (*key != '\0' && (trie = trie->next[strToInt(key)]) != NULL) {
    ++currentDepth;
    ++key;
    if (trie->value != NULL) {
      *n = currentDepth;
      result = trie->value;
    }
  }
  return result;
}

Vector *trieReverse(Trie const *trie, char const *val) {
  Vector *result = vectorNew();
  if (result == NULL) {
    return NULL;
  }

  // dodajemy do vectora przetworzone wartości z list keys należących do
  // wierzchołków na ścieżce do wierzchołka, którego kluczem jest val
  while (*val != '\0' && (trie = trie->next[strToInt(val)]) != NULL) {
    ++val;
    if (listEmpty(trie->keys)) {
      continue;
    }

    ListIterator *iter = listIteratorNew(trie->keys);
    if (iter == NULL) {
      vectorDelete(result);
      return NULL;
    }

    // iterowanie po wszystkich napisach z listy
    do {
      char *str = strConcat(listIteratorValue(iter), val);
      if (str == NULL || !vectorAdd(result, str)) {
        free(str);
        vectorDelete(result);
        listIteratorDelete(iter);
        return NULL;
      }
    } while (listIteratorNext(iter));

    listIteratorDelete(iter);
  }

  return result;
}

void trieRemove(Trie *trie, char const *key) {
  while (*key != '\0' && trie != NULL) {
    trie = trie->next[strToInt(key)];
    ++key;
  }

  if (trie != NULL) {
    trieRemoveValues(trie);
  }
}
