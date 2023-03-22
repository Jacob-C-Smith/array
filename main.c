// Standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <array/array.h>

// Forward declaration
int print_all_elements(array* a);

// Entry point
int main(int argc, const char* argv[])
{

    // Initialized data
    char     *elements[]  = { "Dogs", "Cats", "Birds", "Fish", (void*)0 };
    array    *p_array     = (void *) 0;

    // Make an array with 4 elements
    array_construct(&p_array, 4);

    // Insert some items
    array_add(p_array, "Red"  );
    array_add(p_array, "Green");
    array_add(p_array, "Blue" );

    // Print the arrays' keys
    print_all_elements(p_array);

    // Destroy the array
    array_destroy(&p_array);
    
    // Success
    return EXIT_SUCCESS;
}

// Print the arrays' elements
int print_all_elements(array* p_array)
{

    if (p_array == 0)
        return 0;

    // Get the arrays' elements
    size_t   element_count = 0;
    void   **pp_elements   = 0;
    size_t  *count         = 0;

    array_get(p_array, 0, element_count);
    pp_elements = calloc(element_count, sizeof(void *));
    array_get(p_array, pp_elements, &count);

    // Print each key
    for (size_t i = 0; i < count; i++)
    {
        printf("[%i] %s\n", i, pp_elements[i]);
    }
    
    // Formatting
    putchar('\n');
    
    // Success
    return 1;
}