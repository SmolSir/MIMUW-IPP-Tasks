#ifndef LINE_PARSING_H
#define LINE_PARSING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * Function called to initialize a new element for the struct array
 * line_representation with the default array sizes and numeric values,
 * except for line_id which is set to current line_number.
 * line_representation - array of structs with information about lines.
 * line_number - number of the current line used for initialization.
 */
void init_line_representation(
    Line_elements *line_representation,
    size_t line_number);

/**
 * Function taking the current line and parsing it to pass all the
 * information about the line to line_representation struct, so that
 * all data is uniform easily accessible for later operations.
 * line - string of the current line's characters.
 * line_length - number of characters in the current line.
 * not_numbers - array of words that are not numbers.
 * total_not_numbers - number of elements in the not_numbers array.
 * numbers - array of words that are numbers, kept as long doubles.
 * total_numbers - number of elements in the numbers array.
 */
void parse_line(
    char *line,
    size_t line_length,
    char ***not_numbers,
    size_t *total_not_numbers,
    long double **numbers,
    size_t *total_numbers);

#endif