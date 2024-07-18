# array
[![CMake](https://github.com/Jacob-C-Smith/array/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Jacob-C-Smith/array/actions/workflows/cmake.yml)

**Dependencies:**\
[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)
[![log](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml)

 A minimal array implementation written in C. 

 > 0 [Try it](#try-it)
 >
 > 1 [Download](#download)
 >
 > 2 [Build](#build)
 >
 > 3 [Example](#example)
 >
 >> 3.1 [Example output](#example-output)
 >
 > 4 [Tester](#tester)
 >
 > 5 [Definitions](#definitions)
 >
 >> 5.1 [Type definitions](#type-definitions)
 >>
 >> 5.2 [Function definitions](#function-definitions)

## Try it
[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Jacob-C-Smith/array?quickstart=1)

Wait for a few moments, then click the play button on the bottom of the window. This will run the example program.

 ## Download
 To download array, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/array
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd array
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build array for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./array_example
 ```
 ### Example output
 ```
 TODO
 ```
 [Source](main.c)
## Tester
 To run the tester program, execute this command after building
 ```
 $ ./array_test
 ```
 [Source](array_test.c)
 
 [Tester output](test_output.txt)
 ## Definitions
 ### Type definitions
 ```c
 typedef struct array_s array;
 ```
 ### Function definitions
 ```c 
// Allocators
int array_create ( array **const pp_array );

// Constructors
int array_construct      ( array **const pp_array, size_t size );
int array_from_elements  ( array **const pp_array, void *const *const elements );
int array_from_arguments ( array **const pp_array, size_t size, size_t element_count, ... )

// Accessors
int    array_index    ( const array *const p_array, signed index, void **const pp_value );
int    array_get      ( const array *const p_array, const void ** const pp_elements, size_t *const p_count );
int    array_slice    ( const array *const p_array, const void ** const pp_elements, signed lower_bound, signed upper_bound );
bool   array_is_empty ( const array *const p_array );
size_t array_size     ( const array *const p_array );

// Mutators
int array_add        ( array *const p_array, void *const p_element );
int array_clear      ( array *const p_array );
int array_free_clear ( array *const p_array, void (*const free_fun_ptr)(void *) );

// Iterators
int array_foreach_i  ( const array *const p_array, void (*const function)(void *const value, size_t index) );

// Destructors
int array_destroy    ( array **const pp_array );
 ```
