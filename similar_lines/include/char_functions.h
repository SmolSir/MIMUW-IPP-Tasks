#ifndef CHAR_FUNCTIONS_H
#define CHAR_FUNCTIONS_H

#include <stdio.h>
#include <stdbool.h>

/**
 * Function returns true if the characters ASCII code is between 33 and 126,
 * that is when the char is legal and not a word separator. Otherwise it
 * returns false.
 * c - given char.
 */
bool standard_char(char c);

/**
 * Function returns true if the character is illegal, that is if it's ASCII
 * code is neither between 9 and 13, nor between 32 and 126. Otherwise it
 * returns false.
 * c - given char.
 */
bool incorrect_char(char c);

/**
 * Function returns a lowercase letter if it receives an uppercase letter as
 * an argument; in other words, it turns letters A - Z into a - z adequately.
 * c - given char.
 */
char standard_format(char c);

#endif