#include "word_parsing.h"
#include "memory_allocation.h"

/**
 * Function checks if the given word (string) is a number in decimal system
 * satisfying the tasks requirements. If it is, function returns true,
 * otherwise returns false.
 * word - given string to check.
 * length - size of given string.
 */
static bool incorrect_octal(char *word, size_t length) {
    
    if (word[0] == '-' || word[0] == '+') {
        
        for (size_t i = 1; i < length; i++) {
            
            if (word[i] < '0' || '7' < word[i]) {
                return false;
            }
        }

        return true;
    }

    return false;
}

/**
 * Function checking special cases for the conversion from word (string)
 * to long long.
 * word - given string to check.
 * length - size of given string.
 */
static bool special_conditions_ll(char *word, size_t length) {

    if (strncmp(word, "-0x", 3) == 0) return false;
    if (strncmp(word, "+0x", 3) == 0) return false;
    if (incorrect_octal(word, length)) return false;
    
    return true;
}

/**
 * Function checking special cases for the conversion from word (string)
 * to unsigned long long.
 * word - given string to check.
 * length - size of given string.
 */
static bool special_conditions_ull(char *word, size_t length) {

    if (strncmp(word, "-", 1) == 0) return false;
    if (strncmp(word, "-0x", 3) == 0) return false;
    if (strncmp(word, "+0x", 3) == 0) return false;
    if (incorrect_octal(word, length)) return false;

    return true;
}

/**
 * Function checking special cases for the conversion from word (string)
 * to long double.
 * word - given string to check.
 */
static bool special_conditions_ld(char *word) {

    if (strncmp(word, "0x", 2) == 0) return false;
    if (strncmp(word, "-0x", 3) == 0) return false;
    if (strncmp(word, "+0x", 3) == 0) return false;
    if (strcmp(word, "nan") == 0) return false;

    return true;
}

/**
 * Function trying to convert the given word (string) to long long.
 * If the conversion was successful, the function returns true.
 * Otherwise it returns false.
 * temp_ll - variable storing the attempt's converted value.
 * word - string to be converted to a number.
 * word_length - size of given string.
 */
static bool correct_ll(long long *temp_ll, char *word, size_t word_length) {
    
    char *readEnd = NULL;
    *temp_ll = strtoll(word, &readEnd, 0);
    
    if (
        errno == 0 &&
        *readEnd == 0 &&
        special_conditions_ll(word, word_length)) 
    {
        return true;
    }

    errno = 0;
    return false;
}

/**
 * Function trying to convert the given word (string) to unsigned long long.
 * If the conversion was successful, the function returns true.
 * Otherwise it returns false.
 * temp_ull - variable storing the attempt's converted value.
 * word - string to be converted to a number.
 * word_length - size of given string.
 */
static bool correct_ull(
    unsigned long long *temp_ull,
    char *word,
    size_t word_length) 
{
    char *readEnd = NULL;
    
    if (strcmp(word, "0x") == 0) {
        *temp_ull = 0;
        return true;
    }

    *temp_ull = strtoull(word, &readEnd, 0);
    
    if (
        errno == 0 &&
        *readEnd == 0 &&
        special_conditions_ull(word, word_length))
    {
        return true;
    }

    errno = 0;
    return false;
}

/**
 * Function trying to convert the given word (string) to long double.
 * If the conversion was successful, the function returns true.
 * Otherwise it returns false.
 * temp_ld - variable storing the attempt's converted value.
 * word - string to be converted to a number.
 */
static bool correct_ld(long double *temp_ld, char *word) {
    
    char *readEnd = NULL;
    *temp_ld = strtold(word, &readEnd);
    
    if (errno == 0 && *readEnd == 0 && special_conditions_ld(word)) {
        return true;
    }

    errno = 0;
    return false;
}

void parse_word(
    char *word,
    size_t word_length,
    char ***not_numbers,
    size_t *total_not_numbers,
    long double **numbers,
    size_t *total_numbers)
{
    long long temp_ll = 0;
    unsigned long long temp_ull = 0;
    long double temp_ld = 0;

    /**
     * Attempts to convert the word (string) to long long,
     * then unsigned long long, and lastly double:
     */
    if (correct_ll(&temp_ll, word, word_length)) {
        (*numbers)[*total_numbers] = (long double)(temp_ll);
        (*total_numbers)++;
        return;
    }

    if (correct_ull(&temp_ull, word, word_length)) {
        (*numbers)[*total_numbers] = (long double)(temp_ull);
        (*total_numbers)++;
        return;
    }

    if (correct_ld(&temp_ld, word)) {
        (*numbers)[*total_numbers] = temp_ld;
        (*total_numbers)++;
        return;
    }

    /**
     * Since the word (string) was not converted to a number, it is classified
     * as not a number.
     */
    (*not_numbers)[*total_not_numbers] = 
        (char*)allocate_memory(
            word_length + 1,
            sizeof(char)
        );
    
    memcpy((*not_numbers)[*total_not_numbers], word, word_length);

    (*total_not_numbers)++;
}