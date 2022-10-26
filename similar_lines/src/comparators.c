#include "comparators.h"
#include "structures.h"

int ldouble_compare(const void *ld_a, const void *ld_b) {
    
    if (*(long double*)ld_a == *(long double*)ld_b) return 0;
    
    return *(long double*)ld_a < *(long double*)ld_b ? (-1) : 1;
}

int string_compare(const void *a, const void *b) {
    
    char *str_a = *(char**)a;
    char *str_b = *(char**)b;

    return strcmp(str_a, str_b);
}

int string_array_compare(char **arr_1, char **arr_2, size_t arr_size) {
    
    for (size_t i = 0; i < arr_size; i++) {
        
        int result = strcmp(arr_1[i], arr_2[i]);
        
        if (result != 0) {
            return result;
        }
    }

    return 0;
}

int ldouble_array_compare(
    long double *arr_1,
    long double *arr_2,
    size_t arr_size) 
{
    
    for (size_t i = 0; i < arr_size; i++) {
        
        if ((arr_1)[i] == (arr_2)[i]) {
            continue;
        }

        return (arr_1)[i] < (arr_2)[i] ? (-1) : 1;
    }

    return 0;
}

int line_compare(const void *a, const void *b) {
    Line_elements *line_1 = (Line_elements*)a;
    Line_elements *line_2 = (Line_elements*)b;


            
    if (line_1->numbers_size == line_2->numbers_size) {
        
        if (line_1->not_numbers_size == line_2->not_numbers_size) {
            
            int numbers_compare_result = 
                ldouble_array_compare(
                    line_1->numbers,
                    line_2->numbers,
                    line_1->numbers_size
                );

            if (numbers_compare_result == 0) {

                return string_array_compare(
                    line_1->not_numbers,
                    line_2->not_numbers,
                    line_1->not_numbers_size
                );
            
            }

            return numbers_compare_result;
        }

        return line_1->not_numbers_size < line_2->not_numbers_size ? (-1) : 1;
    }

    return line_1->numbers_size < line_2->numbers_size ? (-1) : 1;
}