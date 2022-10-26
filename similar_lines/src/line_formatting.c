#include "line_formatting.h"
#include "char_functions.h"

bool bad_input_line(char *line, size_t length, size_t line_number) {
    
    if (line[0] == '#') {
        return true;
    }
    
/**
 * Variable to hold information whether there are any words inside the line,
 * that is at least one character with ASCII code in range 33 - 126 is found.
 */
    bool only_line_separators = true;

    for (size_t i = 0; i < length; i++) {
        
        if (standard_char(line[i])) {
            
            only_line_separators = false;
        }

        if (incorrect_char(line[i])) {

            fprintf(stderr, "ERROR %ld\n", line_number);
            
            return true;
        }
    }
    
    return only_line_separators;
}

void lowercase_letters(char **line, size_t line_length) {
    
    for (size_t i = 0; i < line_length; i++) {
        
        (*line)[i] = standard_format((*line)[i]);
    }
}