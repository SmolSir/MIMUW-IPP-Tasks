#ifndef OUTPUT_RECOVERY_H
#define OUTPUT_RECOVERY_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Function called to print the final answer. Since it is required for the
 * answered lines to be sorted in ascending order, first for loop finds the
 * smallest, not yet printed line number that is a valid line (indicated by)
 * non-zero value of it's next_equal_line_id value), and then the path created
 * by the next_equal_line_id values is printed in the same line (the ascending
 * order of the answer is guaranteed by sorting during the computation of the
 * array). Once the end of a path is reached (indicated by value of
 * next_equal_line_id at index i being equal to i), a newline is printed and
 * the for loop continues to the next yet not printed element.
 * next_equal_line_id - array indicating similarity of lines.
 * line_number - size of the next_equal_line_id array.
 */
void recover_output(size_t **next_equal_line_id, size_t line_number);

#endif