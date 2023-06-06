#include <array/array.h>

// Platform dependent includes
#ifdef _WIN64
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif

// Platform dependent macros
#ifdef _WIN64
#define mutex_t HANDLE
#else
#define mutex_t pthread_mutex_t
#endif

struct array_s {
    size_t               element_count, // elements
                         iterable_max;  // Iterable array bound
    mutex_t              lock;          // Locked when writing values
    void               **elements;      // Iterable elements
};

int create_mutex ( mutex_t *p_mutex )
{
    #ifdef _WIN64
        *p_mutex = CreateMutex(0, FALSE, 0);
        return ( p_mutex != 0 );
    #else
        return ( pthread_mutex_init(p_mutex, NULL) == 0 );
    #endif

    return 0;
}

int lock_mutex   ( mutex_t *p_mutex )
{
    #ifdef _WIN64
        return ( WaitForSingleObject(*p_mutex, INFINITE) == WAIT_FAILED ? 0 : 1 );
    #else
        return ( pthread_mutex_lock(p_mutex) == 0 );
    #endif

    return 0;
}

int unlock_mutex ( mutex_t *p_mutex )
{
    #ifdef _WIN64
        return ReleaseMutex(*p_mutex);
    #else
        return ( pthread_mutex_unlock( p_mutex ) == 0 );
    #endif

    return 0;
}

int destroy_mutex ( mutex_t *p_mutex )
{
    #ifdef _WIN64
        return ( CloseHandle(*p_mutex) );
    #else
        return ( pthread_mutex_destroy(p_mutex) == 0 );
    #endif

    return 0;
}

int array_create ( array **pp_array )
{

    // Argument check
    {
        if ( pp_array == (void *) 0 )
            goto no_array;
    }

    // Allocate memory for an array
    array *p_array = calloc(1, sizeof(array));

    // Error checking
    {
        if( p_array == (void *) 0 )
            goto no_mem;
    }

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
        if ( pp_array == (void *) 0 )
            goto no_array;
        if (size == 0)
            goto zero_size;
    }

    // Initialized data
    array *p_array = 0;

    // Allocate am array
    if ( array_create(pp_array) == 0 )
        goto failed_to_create_array;
    
    // Get a pointer to the allocated array
    p_array               = *pp_array;

    // Set the count and iterator max
    p_array->iterable_max = 1;

    // Allocate "size" number of properties
    p_array->elements      = calloc(size, sizeof(void *));

    // Create a mutex
    if ( create_mutex(&p_array->lock) == 0 )
        goto failed_to_create_mutex;

    // Error checking
    {
        if ( p_array->elements == (void *) 0 )
            goto no_mem;
    }

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
        if ( pp_array == (void *) 0 )
            goto no_array;
        if ( elements == (void *) 0 )
            goto no_elements;
    }

    // Initialized data
    array  *p_array       = 0;
    size_t  element_count = 0;

    // Count elements
    while(elements[++element_count]);

    // Allocate an array
    if ( array_construct(&p_array, element_count) == 0 )

        // Allocation failed
        return 0;

    // Iterate over each key
    for (size_t i = 0; elements[i]; i++)
    {

        // Add the key to the array
        array_add(p_array, elements[i]);
    }

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
    {
        *pp_value = p_array->elements[index];
    }

    // Negative numbers
    else 
    {
        *pp_value = p_array->elements[p_array->element_count - ( index * -1 )];
    }

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
        if( p_array == (void *) 0 )
            goto no_array;
    }

    // Lock
    lock_mutex(&p_array->lock);

    // Return
    if(pp_elements)
    {
        memcpy(pp_elements, p_array->elements, sizeof(void *)*p_array->element_count);
    }

    if (p_count)
        *p_count = p_array->element_count;

    // Unlock
    unlock_mutex(&p_array->lock);

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
    return 1;

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
    return 1;

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
        if(p_array == (void *)0)
            goto no_array;
    }

    // Lock
    lock_mutex(&p_array->lock);

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
        p_array->elements   = realloc(p_array->elements, p_array->iterable_max * sizeof(void *));
    
        // Error checking
        {
            if ( p_array->elements == (void *) 0 )
                goto no_mem;
        }
    }

    // Unlock
    unlock_mutex(&p_array->lock);

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
                unlock_mutex(&p_array->lock);

                // Error
                return 0;
        }
    }
}

int array_clear ( array *p_array )
{

    // Argument check
    {
        if(p_array == (void *)0)
            goto no_array;
    }

    // Lock
    lock_mutex(&p_array->lock);

    // Clear the entries
    memset(p_array->elements, 0, sizeof(void*)*p_array->iterable_max);

    // Clear the element counter
    p_array->element_count = 0;

    // Unlock
    unlock_mutex(&p_array->lock);

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
        if( p_array == (void *)0 )
            goto no_array;
        if( free_fun_ptr == (void *)0 )
            goto no_free_func;
        
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
        if( p_array == (void *)0 )
            goto no_array;
        if( function == (void *)0 )
            goto no_free_func;
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
    lock_mutex(&p_array->lock);

    // No more pointer for end user
    *pp_array = (array *) 0;

    // Unlock
    unlock_mutex(&p_array->lock);

    // Free the hash table
    free(p_array->elements);

    // Destroy the mutex
    destroy_mutex(&p_array->lock);

    // Free the array
    free(p_array);
    
    // Success
    return 1;

    // Error handling
    {
        no_array:
            #ifndef NDEBUG
                printf("[array] Null pointer provided for \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}
