/** @file
  Interfejs do zarządzania pamięcią tablic.

  @authors Bartosz Smolarczyk <b.smolarczyk@student.uw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef __MEMORY_H__
#define __MEMORY_H__

/**
 * To jest początkowy, a następnie minimalny rozmiar tablic dynamicznych.
 */
#define INIT_ARRAY_SIZE 32

#include <stdio.h>
#include <stdlib.h>

/**
 * Alokuje pamięć dla określonej liczby elementów zadanego rozmiaru.
 * Jeżeli alokacja nie powiodła się, kończy program kodem błędu 1.
 * @param[in] arr_size : rozmiar tablicy
 * @param[in] elem_size : rozmiar pojedynczego elementu (w bajtach)
 * @return wskaźnik na zaalokowaną pamięć
 */
static inline void *MemAlloc(size_t arr_size, size_t elem_size) {
    void *new_arr = malloc(arr_size * elem_size);
    if (!new_arr) exit(1);
    return new_arr;
}

/**
 * Realokuje pamięć pod danym wskaźnikiem do określonej liczby elementów
 * zadanego rozmiaru. Jeżeli realokacja nie powiodła się, kończy program kodem
 * błędu 1.
 * @param[in] ptr : wskaźnik na pamięć do realokacji
 * @param[in] arr_size : rozmiar tablicy
 * @param[in] elem_size : rozmiar pojedynczego elementu (w bajtach)
 * @return wskaźnik na realokowaną pamięć
 */
static inline void *MemRealloc(void *ptr, size_t arr_size, size_t elem_size) {
    void *new_arr = realloc(ptr, arr_size * elem_size);
    if (!new_arr) exit(1);
    return new_arr;
}

#endif /* __MEMORY_H__ */
