/** @file
 * Implementacja interfejsu klasy przechowującej przekierowania
 * numerów telefonicznych.
 *
 * @date 2022
 */

#include "phone_forward.h"
#include <stdlib.h>
#include <string.h>
#include "string_utils.h"
#include "trie.h"
#include "vector.h"

/**
 * Struktura przechowująca przekierowania numerów telefonów.
 */
struct PhoneForward {
  Trie *trie;  ///< Drzewo przekierowań.
};

/**
 * Struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers {
  Vector *vector;  ///< Vector przechowujący napisy.
};

/** @brief Sprawdza, czy numer telefonu jest prawidłowy.
 * Prawidłowy numer to niepusty napis składający się ze znaków '0'-'9'
 * oraz '*' i '#'.
 * @param[in] num - wskaźnik na napis do sprawdzenia.
 * @return Wartość @p true, jeśli numer jest prawidłowy i wartość @p false,
 *         jeśli nie jest.
 */
static bool isPhoneNumberCorrect(char const *num) {
  if (num == NULL) {
    return false;
  }

  do {
    if (*num != '*' && *num != '#' && (*num < '0' || *num > '9')) {
      return false;
    }
  } while (*++num != '\0');

  return true;
}

/** @brief Porównuje dwa numery telefonów.
 * Przyjmujemy, że znak '*' jest większy niż znak '9' i mniejszy niż znak '#'.
 * @param[in] a – wskaźnik na wskaźnik na pierwszy numer.
 * @param[in] b – wskaźnik na wskaźnik na drugi numer.
 * @return Wartość <0, jeśli pierwszy napis jest leksykograficznie mniejszy niż
 *         drugi, wartość >0, jeśli jest większy i wartość 0, jeśli są równe.
 */
static int phoneNumbersComparator(void const *a, void const *b) {
  char const *num1 = *(char const **)a, *num2 = *(char const **)b;
  while (*num1 != '\0' && *num2 != '\0') {
    if (*num1 != *num2) {
      return strToInt(num1) < strToInt(num2) ? -1 : 1;
    }
    ++num1;
    ++num2;
  }
  return *num1 == *num2 ? 0 : (*num2 == '\0' ? 1 : -1);
}

/** @brief Tworzy nową strukturę przechowującą ciąg numerów.
 * Jeśli nie udało się alokować pamięci, zwalnia vector @p v.
 * @param[in] v - wskaźnik na vector przechowujący ciąg numerów telefonów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci albo @p v ma wartość NULL.
 */
static PhoneNumbers *phnumNew(Vector *v) {
  if (v == NULL) {
    return NULL;
  }

  PhoneNumbers *pn = (PhoneNumbers *)malloc(sizeof(PhoneNumbers));
  if (pn != NULL) {
    pn->vector = v;
  } else {
    vectorDelete(v);
  }

  return pn;
}

PhoneForward *phfwdNew(void) {
  PhoneForward *pf = (PhoneForward *)malloc(sizeof(PhoneForward));
  if (pf != NULL && (pf->trie = trieNew()) == NULL) {
    phfwdDelete(pf);
    pf = NULL;
  }

  return pf;
}

void phfwdDelete(PhoneForward *pf) {
  if (pf != NULL) {
    trieDelete(pf->trie);
    free(pf);
  }
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
  if (!isPhoneNumberCorrect(num1) || !isPhoneNumberCorrect(num2) ||
      strcmp(num1, num2) == 0 || pf == NULL) {
    return false;
  }

  char *num1Copy = strCopy(num1);
  char *num2Copy = strCopy(num2);
  if (num1Copy == NULL || num2Copy == NULL ||
      !trieInsert(pf->trie, num1Copy, num2Copy)) {
    free(num1Copy);
    free(num2Copy);
    return false;
  }

  return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
  if (pf != NULL && isPhoneNumberCorrect(num)) {
    trieRemove(pf->trie, num);
  }
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
  if (pf == NULL) {
    return NULL;
  }

  Vector *v = vectorNew();
  if (v == NULL || !isPhoneNumberCorrect(num)) {
    return phnumNew(v);
  }

  char *result;
  size_t oldPrefixLength = 0;
  char *newPrefix = trieGet(pf->trie, num, &oldPrefixLength);
  if (newPrefix != NULL) {
    // numer został przekierowany
    size_t newPrefixLength = strlen(newPrefix);
    size_t resultLength = strlen(num) - oldPrefixLength + newPrefixLength;

    result = (char *)malloc((resultLength + 1) * sizeof(char));
    if (result != NULL) {
      // wynikowy napis to num, w którym zamieniono stary prefiks na nowy
      strcpy(result, newPrefix);
      strcpy(result + newPrefixLength, num + oldPrefixLength);
    }
  } else {
    // numer nie został przekierowany
    result = strCopy(num);
  }

  if (result == NULL || !vectorAdd(v, result)) {
    free(result);
    vectorDelete(v);
    return NULL;
  }

  return phnumNew(v);
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
  if (pf == NULL) {
    return NULL;
  }
  if (!isPhoneNumberCorrect(num)) {
    return phnumNew(vectorNew());
  }

  Vector *result = trieReverse(pf->trie, num);

  // wynik zawsze zawiera num
  char *numCopy = strCopy(num);
  if (result == NULL || numCopy == NULL || !vectorAdd(result, numCopy)) {
    free(numCopy);
    vectorDelete(result);
    return NULL;
  }

  vectorSort(result, phoneNumbersComparator);

  // usuwanie duplikatów z vectora, zachowując kolejność elementów
  // j - liczba różnych napisów na pozycjach mniejszych od i
  size_t size = vectorSize(result), j = 1;
  for (size_t i = 1; i < size; ++i) {
    if (strcmp(vectorGet(result, i - 1), vectorGet(result, i)) != 0) {
      vectorSwap(result, i, j++);
    }
  }
  // duplikaty zostały przeniesione na koniec - możemy je usunąć
  while (j++ < size) {
    vectorRemoveLast(result);
  }

  vectorShrink(result);

  return phnumNew(result);
}

PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num) {
  PhoneNumbers *pn = phfwdReverse(pf, num);
  if (pn == NULL || phnumGet(pn, 0) == NULL) {
    return pn;
  }

  // usuwanie z vectora elementów x, dla których phfwdGet(x) != num
  // kolejność leksykograficzna jest zachowana
  // j - liczba napisów na pozycjach <i, dla których phfwdGet(x) = num
  size_t size = vectorSize(pn->vector), j = 0;
  for (size_t i = 0; i < size; ++i) {
    PhoneNumbers *pnGet = phfwdGet(pf, phnumGet(pn, i));
    if (pnGet == NULL) {
      phnumDelete(pnGet);
      phnumDelete(pn);
      return NULL;
    }

    if (phnumGet(pnGet, 0) != NULL && strcmp(phnumGet(pnGet, 0), num) == 0) {
      vectorSwap(pn->vector, i, j++);
    }
    phnumDelete(pnGet);
  }
  // złe numery zostały przeniesione na koniec - możemy je usunąć
  while (j++ < size) {
    vectorRemoveLast(pn->vector);
  }

  vectorShrink(pn->vector);

  return pn;
}

void phnumDelete(PhoneNumbers *pnum) {
  if (pnum != NULL) {
    vectorDelete(pnum->vector);
    free(pnum);
  }
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
  return pnum != NULL ? vectorGet(pnum->vector, idx) : NULL;
}
