// Standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <array/array.h>

// Forward declaration
int print_all_elements(array* a);

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

    // Insert some items
    array_add(p_array, "Red");
    array_add(p_array, "Green");
    array_add(p_array, "Blue");

    // Print the arrays' keys
    print_all_elements(p_array);

    array_slice(p_array, (void**)slice_of_array, 1, 2);

    printf("%s\n",slice_of_array[0]);
    printf("%s\n",slice_of_array[1]);
    
    // Destroy the array
    array_destroy(&p_array);
    
    // Success
    return EXIT_SUCCESS;
}

// Print the arrays' elements
int print_all_elements(array* p_array)
{

    if ( p_array == 0 )
        return 0;

    // Get the arrays' elements
    void   **pp_elements   = 0;
    size_t   count         = 0;

    array_get(p_array, 0, &count);
    pp_elements = calloc(count, sizeof(void *));
    array_get(p_array, pp_elements, 0);

    // Iterate over each element
    for (size_t i = 0; i < count; i++)

        // Print each key
        printf("[%zu] %s\n", i, (char *)pp_elements[i]);
    
    // Formatting
    putchar('\n');
    
    // Success
    return 1;
}
