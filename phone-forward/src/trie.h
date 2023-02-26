/** @file
 * Interfejs klasy definiującej drzewo trie.
 *
 * @date 2022
 */

#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdbool.h>
#include <stddef.h>
#include "vector.h"

/**
 * @brief Struktura reprezentująca drzewo trie.
 * Klucze w tym drzewie to napisy składające się ze znaków '0'-'9'
 * oraz '*' i '#'.
 */
typedef struct Trie Trie;

/** @brief Tworzy nowe drzewo trie.
 * Tworzy nowe drzewo trie, które składa się tylko z korzenia.
 * @return Wskaźnik na utworzone drzewo lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
Trie *trieNew(void);

/** @brief Usuwa drzewo trie.
 * Nic nie robi, jeśli wskaźnik @p trie jest równy NULL.
 * @param[in] trie – wskaźnik na usuwaną strukturę.
 */
void trieDelete(Trie *trie);

/** @brief Dodaje pod kluczem @p key wartość @p val.
 * Gdy pod kluczem @p key była już dodana wartość, zastępuje ją. Jeśli
 * nie uda się alokować pamięci, pozostawia strukturę bez zmian.
 * @param[in,out] trie - wskaźnik na drzewo trie;
 * @param[in] key - wskaźnik na niepusty napis reprezentujący klucz;
 * @param[in] val - wskaźnik na napis reprezentujący wartość do wstawienia.
 * @return Wartość @p true jeśli wartość została dodana.
 *         Wartość @p false, gdy nie udało się alokować pamięci.
 */
bool trieInsert(Trie *trie, char *key, char *val);

/** @brief Znajduje długość najdłuższego niepustego prefiksu klucza @p key,
 *         z którym jest powiązana jakaś wartość.
 * @param[in] trie - wskaźnik na drzewo trie;
 * @param[in] key  - wskaźnik na niepusty napis reprezentujący klucz.
 * @return Długość szukanego prefiksu lub wartość 0, jeśli taki prefiks
 *         nie istnieje.
 */
size_t trieFind(Trie const *trie, char const *key);

/** @brief Znajduje wartość powiązaną z najdłuższym prefiksem klucza @p key.
 * @param[in] trie - wskaźnik na drzewo trie;
 * @param[in] key - wskaźnik na niepusty napis;
 * @param[in,out] n - wskaźnik na liczbę, której wartość zostanie ustawiona
 *                     na długość prefiksu klucza @p key, z którym jest
 *                     powiązana znaleziona wartość lub 0 jeśli nie istnieje.
 * @return Wskaźnik na napis będący szukaną wartością lub NULL jeśli
 *         taka wartość nie istnieje.
 */
char *trieGet(Trie const *trie, char const *key, size_t *n);

/** @brief Znajduje napisy, które są powiązane z napisem @p val.
 * Napis jest powiązany z napisem @p val, jeśli zamieniając jego pewien
 * prefiks na wartość tego prefiksu w @p trie, otrzymamy @p val.
 * @param[in] trie - wskaźnik na drzewo trie;
 * @param[in] val  - wskaźnik na napis;
 * @return Wskaźnik na vector przechowujący znalezione napisy.
 *         Wartość NULL, jeśli nie udało się alokować pamięci.
 */
Vector *trieReverse(Trie const *trie, char const *val);

/** @brief Usuwa wartości, których prefiksem klucza jest @p key.
 * @param[in,out] trie - wskaźnik na drzewo trie;
 * @param[in] key - wskaźnik na niepusty napis reprezentujący klucz.
 */
void trieRemove(Trie *trie, char const *key);

#endif /* __TRIE_H__ */
