#include "word_parsing.h"
#include "structures.h"
#include "comparators.h"
#include "char_functions.h"
#include "memory_allocation.h"

/**
 * Function returning the next word extracted from the line. It receives
 * index of the char frpm which the word begins and finds it's length by
 * running a while loop until a char which ASCII code lies outside 33 - 126
 * range is found.
 * line - string of the current line's characters.
 * line_iter - the first character of the currently read word.
 * word_length - length of the currently read word.
 */
static char *get_word(char *line, size_t *line_iter, size_t *word_length) {
    while (standard_char(line[*line_iter])) {
        (*line_iter)++;
        (*word_length)++;
    }

    char *word = (char*)allocate_memory(*word_length + 1, sizeof(char));
    memcpy(word, line + *line_iter - *word_length, *word_length);
    
    return word;
}

void init_line_representation(
    Line_elements *line_representation,
    size_t line_number)
{
    line_representation->numbers =
        (long double*)allocate_memory(INITIAL_ARRAY_SIZE, sizeof(long double));

    line_representation->not_numbers =
        (char**)allocate_memory(INITIAL_ARRAY_SIZE, sizeof(char*));

    line_representation->line_id = line_number;
    
    line_representation->numbers_size = 0;
    
    line_representation->not_numbers_size = 0;
}

void parse_line(
    char *line,
    size_t line_length,
    char ***not_numbers,
    size_t *total_not_numbers,
    long double **numbers,
    size_t *total_numbers)
{
    size_t not_numbers_size = INITIAL_ARRAY_SIZE;
    size_t numbers_size = INITIAL_ARRAY_SIZE;

    for (size_t i = 0; i < line_length; i++) {
        
        if(standard_char(line[i]) == false) {
            continue;
        }

        else {
            size_t word_length = 0;
            char *single_word = get_word(line, &i, &word_length);

            *(not_numbers) =
                (char**)realloc_if_needed(
                    *not_numbers,
                    *total_not_numbers,
                    &not_numbers_size,
                    sizeof(char*)
                );
            
            *(numbers) = 
                (long double*)realloc_if_needed(
                    *numbers,
                    *total_numbers,
                    &numbers_size,
                    sizeof(long double)
                );

            parse_word(
                single_word,
                word_length,
                not_numbers,
                total_not_numbers,
                numbers,
                total_numbers
            );

            free(single_word);
        }
    }

    *(not_numbers) = (char**)realloc_to_length(*not_numbers, *total_not_numbers, sizeof(char*));
    *(numbers) = (long double*)realloc_to_length(*numbers, *total_numbers, sizeof(long double));

    qsort(*numbers, *total_numbers, sizeof(long double), ldouble_compare);
    qsort(*not_numbers, *total_not_numbers, sizeof(char*), string_compare);
}
