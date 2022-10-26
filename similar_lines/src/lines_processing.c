#include "structures.h"
#include "comparators.h"
#include "memory_allocation.h"

/**
 * Function to free all memory previously allocated that is related to the
 * line_representation array of structs.
 * line_representation - array of structs with information about lines.
 * valid lines - size of line_representation array.
 */
void free_line_representation(
    Line_elements **line_representation,
    size_t valid_lines) 
{
    for (size_t i = 0; i < valid_lines; i++) {

        for (size_t j = 0; j < (*line_representation)[i].not_numbers_size; j++)
        {
            free((*line_representation)[i].not_numbers[j]);
        }
        
        free((*line_representation)[i].numbers);
        free((*line_representation)[i].not_numbers);
    }
    
    free(*line_representation);
}

/**
 * Function to fill the next_equal_line_id. The array is filled by iterating
 * over the elements of line_representation. If the value in the
 * next_equal_line_id for index i is equal to i, then there is no following
 * similar line for line i. Otherwise the following similar line number is
 * the value of the array's element.
 * next_equal_line_id - array to be filled by the function.
 * line_representation - array of structs with information about lines.
 * valid_lines - size of line_representation array.
 */
void compute_next_equal_line_id(
    size_t **next_equal_line_id,
    Line_elements **line_representation,
    size_t valid_lines)
{
    for (size_t i = 1; i < valid_lines; i++) {
            
        if (line_compare(
            &(*line_representation)[i - 1],
            &(*line_representation)[i]) 
            == 0)
        {
            (*next_equal_line_id)[(*line_representation)[i - 1].line_id] = 
                (*line_representation)[i].line_id;
        }
        else 
        {
            (*next_equal_line_id)[(*line_representation)[i - 1].line_id] =
                (*line_representation)[i - 1].line_id;
        }
    }

    /**
     * The last element of the array has to be set manually since there is
     * no latter element for it to refer to.
     */
    (*next_equal_line_id)[(*line_representation)[valid_lines - 1].line_id] =
        (*line_representation)[valid_lines - 1].line_id;
}

void process_lines(
    size_t **next_equal_line_id,
    Line_elements **line_representation,
    size_t valid_lines)
{
    *line_representation =
        (Line_elements*)realloc_to_length(
            *line_representation,
            valid_lines,
            sizeof(Line_elements)
        );
    
    qsort(
        *line_representation,
        valid_lines,
        sizeof(Line_elements),
        line_compare
    );

    compute_next_equal_line_id(
        next_equal_line_id,
        line_representation,
        valid_lines
    );

    free_line_representation(line_representation, valid_lines);
}