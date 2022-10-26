#ifndef WORD_PARSING_H
#define WORD_PARSING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

/**
 * Function determining whether the given word (string) is a number or not.
 * If it is, it's long double representation is added to the numbers
 * array for the current line. If it's not, it is added to the not_numbers
 * array for current line.
 * word - string to parse.
 * word_length - number of characters in the given string.
 * not_numbers - array of words that are not numbers.
 * total_not_numbers - number of elements in the not_numbers array.
 * numbers - array of words that are numbers, kept as long doubles.
 * total_numbers - number of elements in the numbers array.
 */
void parse_word(
    char *word,
    size_t word_length,
    char ***not_numbers,
    size_t *total_not_numbers,
    long double **numbers,
    size_t *total_numbers);

#endif