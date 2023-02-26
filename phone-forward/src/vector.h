/** @file
 * Interfejs klasy przechowującej ciąg napisów jako tablicę.
 *
 * @date 2022
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdbool.h>
#include <stddef.h>

/**
 * Struktura przechowująca ciąg napisów.
 */
typedef struct Vector Vector;

/** @brief Tworzy nową strukturę niezawierającą żadnych elementów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
Vector *vectorNew(void);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p v. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] v – wskaźnik na usuwany vector.
 */
void vectorDelete(Vector *v);

/** @brief Dodaje element na koniec.
 * Jeśli nie uda się zaalokować pamięci, struktura pozostaje niezmieniona.
 * @param[in,out] v – wskaźnik na vector;
 * @param[in] element – wskaźnik na element do dodania.
 * @return Wartość @p true, jeśli element został dodany. Wartość @p false,
 *         jeśli nie udało się alokować pamięci lub @p v ma wartość NULL.
 */
bool vectorAdd(Vector *v, char *element);

/** @brief Znajduje element o indeksie @p idx.
 * Elementy są indeksowane od zera.
 * @param[in] v – wskaźnik na vector;
 * @param[in] idx – indeks szukanego elementu.
 * @return Wskaźnik na szukany element. Wartość NULL jeśli wskaźnik @p v
 *         ma wartość NULL lub @p idx ma za dużą wartość.
 */
char const *vectorGet(Vector const *v, size_t idx);

/** @brief Usuwa ostatni element.
 * Zwalnia pamięć zaalokowaną przez ten element. Jeśli vector jest pusty
 * lub ma wartość NULL, nic nie robi.
 * @param[in,out] v – wskaźnik na vector;
 */
void vectorRemoveLast(Vector *v);

/** @brief Zamienia elementy o podanych indeksach miejscami.
 * Jeśli @p i lub @p j jest za duże lub wskaźnik @p v ma wartość NULL,
 * nic nie robi.
 * @param[in,out] v – wskaźnik na vector;
 * @param[in] i – indeks pierwszego elementu;
 * @param[in] j - indeks drugiego elementu.
 */
void vectorSwap(Vector *v, size_t i, size_t j);

/** @brief Znajduje liczbę elementów w vectorze.
 * @param[in] v – wskaźnik na vector.
 * @return Liczba elementów w vectorze @p v lub 0, gdy wskaźnik @p v ma
 *         wartość NULL.
 */
size_t vectorSize(Vector const *v);

/** @brief Sortuje elementy w strukturze zgodnie z przekazanym komparatorem.
 * @param[in,out] v – wskaźnik na vector;
 * @param[in] cmp - komparator porównujący dwa napisy.
 */
void vectorSort(Vector *v, int (*cmp)(void const *a, void const *b));

/** @brief Zmniejsza pojemność vectora do liczby przechowywanych elementów.
 * Jeśli nie uda się zaalokować pamięci, struktura pozostaje niezmieniona.
 * @param[in,out] v – wskaźnik na vector.
 */
void vectorShrink(Vector *v);

#endif /* __VECTOR_H__ */
