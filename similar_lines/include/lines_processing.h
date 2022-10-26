#ifndef LINES_PROCESSING_H
#define LINES_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Function processing all lines from input and computing the
 * next_equal_line_id array.
 * next_equal_line_id - array to be filled, indicating similarity of lines.
 * line_representation - array of structs with information about lines.
 * valid_lines - size of line_representation array and number of valid lines.
 */
void process_lines(
    size_t **next_equal_line_id,
    Line_elements **line_representation,
    size_t valid_lines);

#endif