#ifndef LINE_FORMATTING_H
#define LINE_FORMATTING_H

#include <stdio.h>
#include <stdbool.h>

/**
 * Function returns true if line is a comment, has illegal characters
 * or does not include any words, that is characters from range 33 - 126.
 * In case of illegal character in a non-commentary line, it prints
 * correct information to stderr.
 * line - string containing characters of the currently processed line.
 * length - the length of the line.
 * line_number - number of the currently processed line.
 */
bool bad_input_line(char *line, size_t length, size_t line_number);

/**
 * Function taking an input string and turning all letters A - Z to a - z
 * adequately.
 * line - string to be processed by the function.
 * line_length - number of characters inside the string.
 */
void lowercase_letters(char **line, size_t line_length);

#endif