/** !
 * array library
 * 
 * @file array.c 
 * 
 * @author Jacob Smith
 */

// Header
#include <array/array.h>

// Structure definitions
struct array_s
{
    size_t    element_count, // Integer quantity of elements
              iterable_max;  // Upper bound of iterable array 
    mutex     _lock;         // Locked when writing values
    void    **elements;      // Array contents
};

// Data
static bool initialized = false;

void array_init ( void ) 
{

    // State check
    if ( initialized == true ) return;

    // Initialize the sync library
    sync_init();
    
    // Initialize the log library
    log_init();

    // Set the initialized flag
    initialized = true;

    // Done
    return; 
}

int array_create ( array **const pp_array )
{

    // Argument check
    if ( pp_array == (void *) 0 ) goto no_array;

    // Allocate memory for an array
    array *p_array = ARRAY_REALLOC(0, sizeof(array));

    // Error checking
    if ( p_array == (void *) 0 ) goto no_mem;

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
                    log_error("[array] Null pointer provided for parameter \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int array_construct ( array **const pp_array, size_t size )
{

    // Argument check
    if ( pp_array == (void *) 0 ) goto no_array;
    if ( size     == 0          ) goto zero_size;

    // Initialized data
    array *p_array = 0;

    // Allocate an array
    if ( array_create(&p_array) == 0 ) goto failed_to_create_array;
    
    // Set the count and iterator max
    p_array->iterable_max = 1;

    // Allocate "size" number of properties
    p_array->elements = ARRAY_REALLOC(0, size * sizeof(void *));

    // Create a mutex
    if ( mutex_create(&p_array->_lock) == 0 ) goto failed_to_create_mutex;

    // Error checking
    if ( p_array->elements == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_array = p_array;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for parameter \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            zero_size:
                #ifndef NDEBUG
                    log_error("[array] Zero provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;   
        }

        // Array errors
        {
            failed_to_create_array:
                #ifndef NDEBUG
                    log_error("[array] Failed to create array in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
            
            failed_to_create_mutex:
                #ifndef NDEBUG
                    log_error("[array] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}

int array_from_elements ( array **const pp_array, void *const *const elements )
{

    // Argument check
    if ( pp_array == (void *) 0 ) goto no_array;
    if ( elements == (void *) 0 ) goto no_elements;

    // Initialized data
    array  *p_array       = 0;
    size_t  element_count = 0;

    // Count elements
    while( elements[++element_count] );

    // Allocate an array
    if ( array_construct(&p_array, element_count) == 0 ) goto failed_to_allocate_array;        

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
                    log_error("[array] Null pointer provided for \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            no_elements:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"keys\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Array errors
        {
            failed_to_allocate_array:
                #ifndef NDEBUG
                    log_error("[array] Call to \"array_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int array_from_arguments ( array **const pp_array, size_t size, size_t element_count, ... )
{

    // Argument check
    if ( pp_array == (void *) 0 ) goto no_array;

    // Uninitialized data
    va_list list;

    // Initialized data
    array *p_array = 0;

    // Initialize the variadic list
    va_start(list, element_count);

    // Allocate an array
    if ( array_construct(&p_array, size) == 0 ) goto failed_to_allocate_array;        

    // Iterate over each key
    for (size_t i = 0; i < element_count; i++)

        // Add the key to the array
        array_add(p_array, va_arg(list, void *));
    
    // Update the element count
    p_array->element_count = element_count;

    // End the variadic list
    va_end(list);

    // Return a pointer to the caller
    *pp_array = p_array;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Array errors
        {
            failed_to_allocate_array:
                #ifndef NDEBUG
                    log_error("[array] Call to \"array_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int array_index ( array *const p_array, signed index, void **const pp_value )
{

    // Argument errors
    if ( p_array                == (void *) 0 ) goto no_array;
    if ( p_array->element_count ==          0 ) goto no_elements;
    if ( pp_value               == (void *) 0 ) goto no_value;

    // Error check
    if ( p_array->element_count == (size_t) abs(index) ) goto bounds_error;

    // Positive index
    if ( index >= 0 )
        *pp_value = p_array->elements[index];

    // Negative numbers
    else 
        *pp_value = p_array->elements[p_array->element_count - (size_t) abs(index)];

    // Success
    return 1;

    // Error handling
    {
        no_array:
            #ifndef NDEBUG
                log_error("[array] Null pointer provided for parameter \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;

        no_value:
            #ifndef NDEBUG
                log_error("[array] Null pointer provided for parameter \"pp_vale\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;

        no_elements:
            #ifndef NDEBUG
                log_error("[array] Can not index an empty array in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error 
            return 0;
        
        bounds_error:
            #ifndef NDEBUG
                log_error("[array] Index out of bounds in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

int array_get ( array *const p_array, void **const pp_elements, size_t *const p_count )
{

    // Argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // Lock
    mutex_lock(&p_array->_lock);

    // Return the elements
    if ( pp_elements )
        memcpy(pp_elements, p_array->elements, sizeof(void *)*p_array->element_count);
    
    // Return the count
    if ( p_count )
        *p_count = p_array->element_count;

    // Unlock
    mutex_unlock(&p_array->_lock);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}

int array_slice ( array *const p_array, void *pp_elements[], signed lower_bound, signed upper_bound )
{

    // Argument check
    if ( p_array == (void *) 0 ) goto no_array;
    if ( lower_bound < 0 ) goto erroneous_lower_bound;
    if ( p_array->element_count < (size_t) upper_bound ) goto erroneous_upper_bound;

    // Lock
    mutex_lock(&p_array->_lock);

    // Return the elements
    if ( pp_elements )
        memcpy(pp_elements, &p_array->elements[lower_bound], sizeof(void *) * (size_t) ( upper_bound - lower_bound + 1LL ) );
    
    // Unlock
    mutex_unlock(&p_array->_lock);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            erroneous_lower_bound:
                #ifndef NDEBUG
                    log_error("[array] Parameter \"lower_bound\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
                
            erroneous_upper_bound:
                #ifndef NDEBUG
                    log_error("[array] Parameter \"upper_bound\" must be less than or equal to array size in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}

bool array_is_empty ( array *const p_array )
{

    // Argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // Success
    return ( p_array->element_count == 0 );

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for parameter \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

size_t array_size ( array *const p_array )
{

    // Argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // Success
    return p_array->element_count;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for parameter \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int array_add ( array *const p_array, void *const p_element )
{

    // Argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // Lock
    mutex_lock(&p_array->_lock);

    // Update the iterables
    p_array->elements[p_array->element_count] = p_element;

    // Increment the entry counter
    p_array->element_count++;

    // Resize iterable max?
    if ( p_array->element_count >= p_array->iterable_max )
    {
    
        // Double the size
        p_array->iterable_max *= 2;
    
        // Reallocate iterable arrays
        p_array->elements = ARRAY_REALLOC(p_array->elements, p_array->iterable_max * sizeof(void *));
    
        // Error checking
        if ( p_array->elements == (void *) 0 ) goto no_mem;
    }

    // Unlock
    mutex_unlock(&p_array->_lock);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Unlock
                mutex_unlock(&p_array->_lock);

                // Error
                return 0;
        }
    }
}

int array_remove ( array *const p_array, signed index, void **const pp_value )
{

    // Argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // State check
    if ( p_array->element_count == 0 ) goto no_elements;
    
    // Initialized data
    size_t _index = 0;

    // Lock
    mutex_lock(&p_array->_lock);

    // Error check
    if ( p_array->element_count == (size_t) abs(index) ) goto bounds_error;

    // Store the correct index
    _index = ( index >= 0 ) ? (size_t) index : (size_t) p_array->element_count - (size_t) abs(index);
    
    // Store the element
    if ( pp_value != (void *) 0 ) *pp_value = p_array->elements[_index];

    // Edge case
    if ( (size_t) index == p_array->element_count-1 ) goto done;

    // Iterate from the index of the removed element to the end of the array
    for (size_t i = _index; i < p_array->element_count-1; i++)
    
        // Shift elements
        p_array->elements[i] = p_array->elements[i+1];

    done:

    // Decrement the element counter
    p_array->element_count--;

    // Unlock
    mutex_unlock(&p_array->_lock);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Array errors
        {
            bounds_error:
                #ifndef NDEBUG
                    log_error("[array] Index out of bounds in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Unlock
                mutex_unlock(&p_array->_lock);

                // Error
                return 0;

            no_elements:
                #ifndef NDEBUG
                    log_error("[array] Can not index an empty array in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        
        }
    }
}

int array_clear ( array *const p_array )
{

    // Argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // Lock
    mutex_lock(&p_array->_lock);

    // Clear the entries
    memset(p_array->elements, 0, sizeof(void*)*p_array->iterable_max);

    // Clear the element counter
    p_array->element_count = 0;

    // Unlock
    mutex_unlock(&p_array->_lock);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int array_free_clear ( array *const p_array, void (*const free_fun_ptr)(void *) )
{

    // Argument check
    if ( p_array      == (void *) 0 ) goto no_array;
    if ( free_fun_ptr == (void *) 0 ) goto no_free_func;

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
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_free_func:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"free_fun_ptr\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
 
int array_foreach_i ( array *const p_array, fn_array_foreach_i *pfn_array_foreach_i ) 
{

    // Argument check
    if ( p_array             == (void *) 0 ) goto no_array;
    if ( pfn_array_foreach_i == (void *) 0 ) goto no_free_func;

    // Iterate over each element in the array
    for (size_t i = 0; i < p_array->element_count; i++)
        
        // Call the function
        pfn_array_foreach_i(p_array->elements[i], i);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_free_func:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int array_log ( array *p_array, void *pfn_next, const char *const format, ... )
{

    // Argument check
    if ( p_array  == (void *) 0 ) goto no_array;
    if ( pfn_next == (void *) 0 ) return 0;

    // Print the header
    log_info("=== %s : %p ===\n", format, p_array);

    // Iterate over each element in the array
    for (size_t i = 0; i < p_array->element_count; i++)
        
        // Call the function
        ;// TODO
        
    // Print a newline
    putchar('\n');

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_log_func:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"pfn_log\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int array_destroy ( array **const pp_array )
{

    // Argument check
    if ( pp_array == (void *) 0 ) goto no_array;

    // Initialized data
    array *p_array = *pp_array;

    // Lock
    mutex_lock(&p_array->_lock);

    // No more pointer for end user
    *pp_array = (array *) 0;

    // Unlock
    mutex_unlock(&p_array->_lock);

    // Free the array contents
    if ( ARRAY_REALLOC(p_array->elements, 0) ) goto failed_to_free;

    // Destroy the mutex
    mutex_destroy(&p_array->_lock);

    // Free the array
    if ( ARRAY_REALLOC(p_array, 0) ) goto failed_to_free;
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
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

void array_exit ( void ) 
{

    // State check
    if ( initialized == false ) return;

    // Clean up sync
    sync_exit();

    // Clean up log
    log_exit();

    // TODO: Anything else?
    //

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}
