/** !
 * Example program for array module
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <array/array.h>

// Forward declaration
void print_array_element ( const void *const p_value, size_t i );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Supress compiler warnings
    (void) argc;
    (void) argv;

    // Initialized data
    array *p_array          = (void *) 0;
    char  *slice_of_array[] = { 0, 0, (void *)0 };

    // Make an array with 4 elements
    array_construct(&p_array, 4);

    // Construct an array
    array_from_arguments(&p_array, 4, 2, "Red", "Yellow");

    // Insert some items
    array_add(p_array, "Green");
    array_add(p_array, "Blue");

    // Print the arrays' elements
    array_foreach_i(p_array, &print_array_element);

    // Get a slice of the array
    array_slice(p_array, (void**)slice_of_array, 1, 2);

    // Formatting
    printf("\nSlice [1..2]\n");

    // Print the array slice
    printf("%s\n",slice_of_array[0]);
    printf("%s\n",slice_of_array[1]);
    
    // Destroy the array
    array_destroy(&p_array);
    
    // Success
    return EXIT_SUCCESS;
}

// Print the arrays' elements
void print_array_element ( const void *const p_value, size_t i )
{

    // Print the element
    printf("[%zu] %s\n", i, (const char *const)p_value);
    
    // Done
    return;
}
