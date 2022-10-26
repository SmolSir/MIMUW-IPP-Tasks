#include "output_recovery.h"

void recover_output(size_t **next_equal_line_id, size_t line_number) {

    for (size_t i = 1; i < line_number; i++) {
            
            if ((*next_equal_line_id)[i] == 0) {
                continue;
            }
            
            size_t current_id = i;
            size_t next_id = (*next_equal_line_id)[current_id];
            
            while (current_id != next_id) {
                
                printf("%ld ", current_id);
                
                (*next_equal_line_id)[current_id] = 0;
                current_id = next_id;
                next_id = (*next_equal_line_id)[current_id];
            }

            printf("%ld\n", current_id);
            
            (*next_equal_line_id)[current_id] = 0;
    }

    free(*next_equal_line_id);
}