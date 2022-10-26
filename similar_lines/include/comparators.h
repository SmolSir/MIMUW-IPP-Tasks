#ifndef COMPARATORS_H
#define COMPARATORS_H

#include <stdio.h>
#include <string.h>

/**
 * Function comparing two numbers of type long double. The returned value is
 * -1 if ld_a < ld_b,
 *  0 if ld_a == ld_b,
 *  1 if ld_a > ld_b.
 * 
 * ld_a - first of the two long doubles.
 * ld_b - second of the two long doubles.
 */
int ldouble_compare(const void *ld_a, const void *ld_b);

/**
 * Function comparing two strings. The returned value is the result of calling
 * the strcmp function for these strings.
 * a - first string.
 * b - second string.
 */
int string_compare(const void *a, const void *b);

/**
 * Function comparing two arrays of strings of equal length. The comparison is
 * done in pairs of strings of equal indexes, until the strings differ 
 * (then function returns -1 or 1, according to strcmp), or 0 if all such
 * pairs are equal.
 * arr_1 - first array of strings.
 * arr_2 - second array of strings.
 * arr_size - size of both string arrays.
 */
int string_array_compare(char **arr_1, char **arr_2, size_t arr_size);

/**
 * Function comparing two arrays of numbers of type long double. The
 * comparison is done in pairs of numbers of equal indexes, until the numbers
 * differ (then function returns -1 if first number is smaller, or 1 vice
 * versa), or 0 if all such pairs are equal.
 * arr_1 - first array of long doubles.
 * arr_2 - second array of long doubles.
 * arr_size - size of both long double arrays.
 */
int ldouble_array_compare(
    long double *arr_1,
    long double *arr_2,
    size_t arr_size);

/**
 * Function comparing two structs representing lines of input. The comparison
 * is done by comparing elements of these structs and returning -1 or 1 if one
 * of them differs, or 0 if all are equal. The comparisons occur in the
 * following order:
 * numbers_size, not_numbers_size, numbers, not_numbers.
 * a - first struct.
 * b - second struct.
 */
int line_compare(const void *a, const void *b);

#endif