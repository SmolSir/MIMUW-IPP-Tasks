#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>

/**
 * Structure used to represent necessary information about
 * a single line of input.
 * numbers - array of all numbers in the line, casted to long doubles.
 * not_numbers - array of all not-numbers in the line.
 * line_id - id of the line, based on input order.
 * numbers_size - size of the numbers array.
 * not_numbers_size - size of the not_numbers array.
 */
typedef struct {
    long double *numbers;
    char **not_numbers;
    int line_id;
    size_t numbers_size;
    size_t not_numbers_size;
}Line_elements;

#endif