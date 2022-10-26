#ifndef MEMORY_ALLOCATION_H
#define MEMORY_ALLOCATION_H

#include <stdio.h>
#include <stdlib.h>

/**
 * The default size of an array if it's final size is unknown.
 */
#define INITIAL_ARRAY_SIZE 4

/**
 * Function to allocate an array of given size for elements of given size.
 * Before returning an array, checks for memory errors.
 * arr_size - size of the returned array.
 * element_size - size of each element of the returned array.
 */
void *allocate_memory(size_t arr_size, size_t element_size);

/**
 * Function to double the size of a given array if it's number of elements
 * is equal to it's size. Before returning an array, checks for memory errors.
 * arr - array of elements of any type.
 * number_of_elements - number of elements inside the array.
 * arr_size - current size of the array to be doubled.
 * element_size - size of each element of the returned array.
 */
void *realloc_if_needed(
    void *arr,
    size_t number_of_elements,
    size_t *arr_size,
    size_t element_size);

/**
 * Function to shorten a given array to the size of it's number of elements
 * to lower memory usage. Before returning an array, checks for memory errors.
 * arr - array of elements of any type.
 * number_of_elements - number of elements inside the array.
 * element_size - size of each element of the returned array.
 */
void *realloc_to_length(
    void *arr,
    size_t number_of_elements,
    size_t element_size);

#endif