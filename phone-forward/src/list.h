/** @file
 * Interfejs klasy przechowującej ciąg napisów jako cykliczną listę.
 *
 * @date 2022
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>

/**
 * Struktura przechowująca ciąg napisów.
 */
typedef struct List List;

/**
 * Struktura przechowująca iterator listy.
 */
typedef struct ListIterator ListIterator;

/** @brief Tworzy nową strukturę niezawierającą żadnych elementów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
List *listNew(void);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p list. Nic nie robi, jeśli wskaźnik
 * ten ma wartość NULL.
 * @param[in] list – wskaźnik na usuwaną listę.
 */
void listDelete(List *list);

/** @brief Dodaje element do listy.
 * Jeśli nie uda się zaalokować pamięci, struktura pozostaje niezmieniona.
 * @param[in,out] list – wskaźnik na listę;
 * @param[in] element – wskaźnik na element do dodania.
 * @return Wskaźnik na listę, której pierwszym elementem jest dodany element.
 *         Wartość NULL, jeśli nie udało się alokować pamięci lub @p list ma
 *         wartość NULL.
 */
List *listAdd(List *list, char *element);

/** @brief Znajduje pierwszy element listy @p list .
 * @param[in] list – wskaźnik na listę.
 * @return Wskaźnik na szukany element. Wartość NULL jeśli wskaźnik @p list
 *         ma wartość NULL lub lista jest pusta.
 */
char *listGet(List const *list);

/** @brief Sprawdza czy lista @p list jest pusta.
 * @param[in] list – wskaźnik na listę.
 * @return Wartość @p true, jeśli lista zawiera 0 elementów lub wskaźnik
 *         @p list ma wartość NULL. Wartość @p false, jeśli lista
 *         zawiera >0 elementów.
 */
bool listEmpty(List const *list);

/** @brief Usuwa element.
 * Usuwa pierwszy element z listy. Jeśli lista jest pusta
 * lub wskaźnik na listę ma wartość NULL, nic nie robi.
 * @param[in,out] list – wskaźnik na listę.
 */
void listRemoveFirst(List *list);

/** @brief Tworzy nowy iterator wskazujący na pierwszy element listy.
 * @param[in] list – wskaźnik na listę.
 * @return Wskaźnik na utworzony iterator lub NULL, gdy nie udało się
 *         alokować pamięci lub lista nie zawiera elementów.
 */
ListIterator *listIteratorNew(List const *list);

/** @brief Posuwa iterator o jeden element.
 * @param[in,out] listIterator – wskaźnik na iterator.
 * @return Wartość @p false, jeśli następny element jest początkowy lub
 *         w liście nie ma elementów. Wartość @p true w przeciwnym razie.
 */
bool listIteratorNext(ListIterator *listIterator);

/** @brief Znajduje element wskazywany przez iterator.
 * @param[in] listIterator – wskaźnik na iterator.
 * @return Element wskazywany przez iterator.
 */
char const *listIteratorValue(ListIterator const *listIterator);

/** @brief Usuwa iterator.
 * Nic nie robi, jeśli wskaźnik ma wartość NULL.
 * @param[in] listIterator – wskaźnik na iterator.
 */
void listIteratorDelete(ListIterator *listIterator);

#endif /* __LIST_H__ */
