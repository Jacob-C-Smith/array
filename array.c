/** !
 * array library
 * 
 * @file array.c 
 * 
 * @author Jacob Smith
 */

// Headers
#include <array/array.h>

// Structure definitions
struct array_s {
    size_t    element_count, // Elements
              iterable_max;  // Iterable array bound
    mutex     _lock;         // Locked when writing values
    void    **elements;      // Iterable elements
};

int array_create ( array **pp_array )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_array == (void *) 0 ) goto no_array;
        #endif
    }

    // Allocate memory for an array
    array *p_array = ARRAY_REALLOC(0, sizeof(array));

    // Error checking
    if ( p_array == (void *) 0 )
        goto no_mem;

    // Zero set
    memset(p_array, 0, sizeof(array));

    // Return the allocated memory
    *pp_array = p_array;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for parameter \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int array_construct ( array **pp_array, size_t size )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_array == (void *) 0 ) goto no_array;
            if ( size     == 0          ) goto zero_size;
        #endif
    }

    // Initialized data
    array *p_array = 0;

    // Allocate am array
    if ( array_create(pp_array) == 0 )
        goto failed_to_create_array;
    
    // Get a pointer to the allocated array
    p_array = *pp_array;

    // Set the count and iterator max
    p_array->iterable_max = 1;

    // Allocate "size" number of properties
    p_array->elements = ARRAY_REALLOC(0, size * sizeof(void *));

    // Create a mutex
    if ( mutex_create(&p_array->_lock) == 0 )
        goto failed_to_create_mutex;

    // Error checking
    if ( p_array->elements == (void *) 0 )
        goto no_mem;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for parameter \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            zero_size:
                #ifndef NDEBUG
                    printf("[array] Zero provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
                
        }

        // Array errors
        {
            failed_to_create_array:
                #ifndef NDEBUG
                    printf("[array] Failed to create array in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
            
            failed_to_create_mutex:
                #ifndef NDEBUG
                    printf("[array] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}

int array_from_elements ( array **pp_array, void **elements )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_array == (void *) 0 ) goto no_array;
            if ( elements == (void *) 0 ) goto no_elements;
        #endif
    }

    // Initialized data
    array  *p_array       = 0;
    size_t  element_count = 0;

    // Count elements
    while( elements[++element_count] );

    // Allocate an array
    if ( array_construct(&p_array, element_count) == 0 )

        // Allocation failed
        return 0;

    // Iterate over each key
    for (size_t i = 0; elements[i]; i++)

        // Add the key to the array
        array_add(p_array, elements[i]);

    // Return
    *pp_array = p_array;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            no_elements:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"keys\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

        }
    }
}

int array_index ( array *p_array, signed index, void **pp_value )
{

    // Argument errors
    {
        #ifndef NDEBUG
            if ( p_array == (void *) 0 ) goto no_array;
            if ( pp_value == (void *) 0 ) goto no_value;
        #endif
    }

    // Positive index
    if ( index <= 0 )
        *pp_value = p_array->elements[index];
    // Negative numbers
    else 
        *pp_value = p_array->elements[p_array->element_count - ( index * -1 )];

    // Success
    return 1;

    // Error handling
    {
        no_array:
            #ifndef NDEBUG
                printf("[array] Null pointer provided for parameter \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error handling
            return 0;

        out_of_bounds:
            #ifndef NDEBUG
                printf("[array] Index out of bounds in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error handling
            return 0;

        no_value:
            #ifndef NDEBUG
                printf("[array] Null pointer provided for parameter \"pp_vale\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error handling
            return 0;

    }
}

int array_get ( array *p_array, void **pp_elements, size_t *p_count )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_array == (void *) 0 ) goto no_array;
        #endif
    }

    // Lock
    mutex_lock(p_array->_lock);

    // Return
    if (pp_elements)
        memcpy(pp_elements, p_array->elements, sizeof(void *)*p_array->element_count);
    
    if (p_count)
        *p_count = p_array->element_count;

    // Unlock
    mutex_unlock(p_array->_lock);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}

bool array_is_empty ( array *p_array )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_array == (void *) 0 ) goto no_array;
        #endif
    }

    // Success
    return ( p_array->element_count == 0 );

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for parameter \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

size_t array_size ( array *p_array )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_array == (void *) 0 ) goto no_array;
        #endif
    }

    // Success
    return p_array->element_count;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for parameter \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int array_add ( array *p_array, void *p_element )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_array == (void *) 0 ) goto no_array;
        #endif
    }

    // Lock
    mutex_lock(p_array->_lock);

    // Update the iterables
    p_array->elements[p_array->element_count] = p_element;

    // Increment the entry counter
    p_array->element_count++;

    // Resize iterable max?
    if ( p_array->element_count >= p_array->iterable_max)
    {
    
        // Double the size
        p_array->iterable_max *= 2;
    
        // Reallocate iterable arrays
        p_array->elements = ARRAY_REALLOC(p_array->elements, p_array->iterable_max * sizeof(void *));
    
        // Error checking
        if ( p_array->elements == (void *) 0 )
            goto no_mem;
    }

    // Unlock
    mutex_unlock(p_array->_lock);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Unlock
                mutex_unlock(p_array->_lock);

                // Error
                return 0;
        }
    }
}

int array_clear ( array *p_array )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_array == (void *) 0 ) goto no_array;
        #endif
    }

    // Lock
    mutex_lock(p_array->_lock);

    // Clear the entries
    memset(p_array->elements, 0, sizeof(void*)*p_array->iterable_max);

    // Clear the element counter
    p_array->element_count = 0;

    // Unlock
    mutex_unlock(p_array->_lock);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int array_free_clear ( array *p_array, void (*free_fun_ptr)(void *) )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_array      == (void *) 0 ) goto no_array;
            if ( free_fun_ptr == (void *) 0 ) goto no_free_func;
        #endif
    }

    // Iterate over each element in the array
    for (size_t i = 0; i < p_array->element_count; i++)
    {
        
        // Call the free function
        free_fun_ptr(p_array->elements[i]);

        // Clear the reference from the array
        p_array->elements[i] = 0;
    }

    // Clear the element counter
    p_array->element_count = 0;

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_free_func:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"free_fun_ptr\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int array_foreach ( array *p_array, void (*function)(void *) )
{

    // Argument check
    {
        if ( p_array  == (void *) 0 ) goto no_array;
        if ( function == (void *) 0 ) goto no_free_func;
    }

    // Iterate over each element in the array
    for (size_t i = 0; i < p_array->element_count; i++)
        
        // Call the free function
        function(p_array->elements[i]);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_free_func:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int array_destroy ( array  **pp_array )
{

    // Argument check
    {
        if ( pp_array == (void *) 0 )
            goto no_array;
    }

    // Initialized data
    array *p_array = *pp_array;

    // Lock
    mutex_lock(p_array->_lock);

    // No more pointer for end user
    *pp_array = (array *) 0;

    // Unlock
    mutex_unlock(&p_array->_lock);

    // Free the array contents
    if ( ARRAY_REALLOC(p_array->elements, 0) )
        goto failed_to_free;

    // Destroy the mutex
    mutex_destroy(&p_array->_lock);

    // Free the array
    if ( ARRAY_REALLOC(p_array, 0) )
        goto failed_to_free;
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            failed_to_free:
                #ifndef NDEBUG
                    printf("[Standard Library] Call to \"realloc\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}
