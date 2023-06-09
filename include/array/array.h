﻿/** !
 * @file array/array.h 
 * 
 * @author Jacob Smith
 * 
 * Include header for array library
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// sync submodule
#include <sync/sync.h>

// Debug mode
#undef NDEBUG

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Memory management macro
#ifndef ARRAY_REALLOC
#define ARRAY_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
struct array_s;

// Type definitions
/** !
 *  @brief The type definition of an array struct
 */
typedef struct array_s array;

// Allocaters
/** !
 *  Allocate memory for an array
 *
 * @param pp_array return
 *
 * @sa array_destroy
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int array_create ( array **pp_array );

// Constructors
/** !
 *  Construct an array with a specific size
 *
 * @param pp_array return
 * @param size number of elements in an array
 *
 * @sa array_create
 * @sa array_destroy
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int array_construct ( array **pp_array, size_t size );

/** !
 *  Construct an array from an array of elements
 *
 * @param pp_array return
 * @param elements pointer to null terminated array of element pointers
 * @param size number of elements. 
 *
 * @sa array_create
 * @sa array_destroy
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int array_from_elements ( array **pp_array, void **elements );

// Accessors
/** !
 * Index an array with a signed number. If index is negative, index = size - |index|, such that
 * [A,B,C,D,E] index(-2) -> D
 * 
 * @param p_array array
 * @param index signed index. 
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error 
 */
DLLEXPORT int array_index ( array *p_array, signed index, void **pp_value );

/** !
 *  Get an array of elements
 *
 * @param p_array array
 * @param pp_elements return
 *
 * @sa array_values
 *
 * @return pointer to specified property's value on success, null pointer on error
 */
DLLEXPORT int array_get ( array *p_array, void **pp_elements, size_t *p_count );

/** !
 *  Is an array empty?
 * 
 * @param p_array an array
 * 
 * @return true if array has no contents else false
 */
DLLEXPORT bool array_is_empty ( array *p_array );

/** !
 *  Get the size of an array
 * 
 * @param p_array an array
 * 
 * @return size of array
 */
DLLEXPORT size_t array_size ( array *p_array );

// Mutators
/** !
 *  Add an element to an array. 
 *
 * @param p_array array
 * @param p_element the value of the element
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int array_add ( array *p_array, void *p_element );

/** !
 *  Clear all elements in an array
 *
 * @param p_array array
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int array_clear ( array *p_array );

/** !
 *  Remove all elements from an array, and deallocate values with free_func
 *
 * @param p_array array
 * @param free_fun_ptr pointer to deallocator function 
 * 
 * @sa array_clear
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int array_free_clear ( array *p_array, void (*free_fun_ptr)(void *) );

// Iterators
/** !
 * Call function on every element in p_array
 *
 * @param p_array array
 * @param function pointer to function of type void (*)(void *value, size_t index)
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int array_foreach_i ( array *p_array, void (*function)(void *value, size_t index) );

// Destructors
/** !
 *  Destroy and deallocate an array
 *
 * @param pp_array array
 *
 * @sa array_create
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int array_destroy ( array **pp_array );