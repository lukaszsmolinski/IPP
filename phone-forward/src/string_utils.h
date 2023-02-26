/** @file
 * Moduł zawierający funkcje pomocnicze przetwarzające napisy.
 *
 * @date 2022
 */

#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/** @brief Kopiuje napis.
 * @param[in] str - wskaźnik na napis do skopiowania.
 * @return Wskaźnik na napis będący kopią napisu @p str
 *         lub NULL, gdy nie udało się alokować pamięci.
 */
static inline char *strCopy(char const *str) {
  char *newStr = (char *)malloc((strlen(str) + 1) * sizeof(char));
  return newStr != NULL ? strcpy(newStr, str) : NULL;
}

/** @brief Tworzy napis, łącząc przekazane napisy.
 * @param[in] str1 - wskaźnik na pierwszy napis;
 * @param[in] str2 - wskaźnik na drugi napis.
 * @return Wskaźnik na utworzony napis lub NULL, gdy
 *         nie udało się alokować pamięci.
 */
static inline char *strConcat(char const *str1, char const *str2) {
  size_t len1 = strlen(str1), len2 = strlen(str2);
  char *result = (char *)malloc((len1 + len2 + 1) * sizeof(char));
  if (result != NULL) {
    strcpy(result, str1);
    strcpy(result + len1, str2);
  }
  return result;
}

/** @brief Zamienia napis na liczbę.
 * Wynikowa liczba zależy od pierwszego znaku napisu. Znaki '0'-'9' są
 * zamieniane na liczby, które reprezentują. Znaki '*' oraz '#' są zamieniane
 * odpowiednio na liczby 10 i 11. Pozostałe znaki są zamieniane na liczbę 12.
 * @param[in] str – napis do zamiany.
 * @return Liczba od 0 do 12.
 */
static inline uint8_t strToInt(char const *str) {
  if (*str >= '0' && *str <= '9') {
    return *str - '0';
  } else if (*str == '*') {
    return 10;
  } else if (*str == '#') {
    return 11;
  }
  return 12;
}

#endif /* __STRING_UTILS_H__ */
