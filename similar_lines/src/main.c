#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "structures.h"
#include "memory_allocation.h"
#include "line_formatting.h"
#include "line_parsing.h"
#include "lines_processing.h"
#include "output_recovery.h"

int main() {

/**
 * line - currently processed line from the input file.
 * len - parameter required by the getline() function.
 * line_length - length of the currently processed line.
 * valid_lines - counter of lines that neither begin with a '#'
 *      nor have a forbidden character and have at least one character
 *      with ASCII code from range 33 - 126.
 * line_number - number of the currently processed line, with the initial value
 *      set to 1 to match the tasks specification.
 * line_representation_size - size of the array line_representation, set to
 *      INITIAL_ARRAY_SIZE. 
 */
    char *line = NULL;
    size_t len = 0;
    size_t line_length = 0;
    size_t valid_lines = 0;
    size_t line_number = 1;
    size_t line_representation_size = INITIAL_ARRAY_SIZE;

/**
 * line_representation - array for storing necessary information about
 *      current line.
 */
    Line_elements *line_representation =
        (Line_elements*)allocate_memory(
            line_representation_size,
            sizeof(Line_elements)
        );

/**
 * while loop to read and process the input file line after line.
 */
    while ((line_length = getline(&line, &len, stdin)) != (size_t) (-1)) {
        
        if (!line) {
            exit(1);
        }
        
        if (bad_input_line(line, line_length, line_number)) {
            line_number++;
            continue;
        }

        lowercase_letters(&line, line_length);

        init_line_representation(
            &line_representation[valid_lines],
            line_number
        );

        parse_line(
            line,
            line_length,
            &(line_representation[valid_lines].not_numbers),
            &(line_representation[valid_lines].not_numbers_size),
            &(line_representation[valid_lines].numbers),
            &(line_representation[valid_lines].numbers_size)
        );

        free(line);
        len = 0;

        valid_lines++;

        line_representation = 
            (Line_elements*)realloc_if_needed(
                line_representation,
                valid_lines,
                &line_representation_size,
                sizeof(Line_elements)
            );

        line_number++;

    }

    free(line);

    if (line_number == 1) {
        free(line_representation);
        return 0;
    }

    size_t *next_equal_line_id =
        (size_t*)allocate_memory(
            line_number,
            sizeof(size_t)
        );

    process_lines(&next_equal_line_id, &line_representation, valid_lines);

    recover_output(&next_equal_line_id, line_number);

    return 0;
}