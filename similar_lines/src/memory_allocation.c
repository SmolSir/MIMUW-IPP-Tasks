#include "memory_allocation.h"

void *allocate_memory(size_t arr_size, size_t element_size) {
    
    void *new_arr = calloc(arr_size, element_size);
    
    if (!new_arr) {
        exit(1);
    }
    
    return new_arr;
}

void *realloc_if_needed(
    void *arr,
    size_t number_of_elements,
    size_t *arr_size,
    size_t element_size) 
{
    if (number_of_elements == *arr_size) {
        *arr_size *= 2;

        void *new_arr = realloc(arr, *arr_size * element_size);
        
        if (!new_arr) {    
            exit(1);
        }

        return new_arr;
    }

    return arr;
}

void *realloc_to_length(
    void *arr,
    size_t number_of_elements,
    size_t element_size)
{
    if (number_of_elements <= INITIAL_ARRAY_SIZE) {
        return arr;
    }
    
    void *new_arr = realloc(arr, number_of_elements * element_size);
    
    if (!new_arr) {
        exit(1);
    }

    return new_arr;
}