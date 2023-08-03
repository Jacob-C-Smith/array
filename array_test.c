/** !
 * @file array_test.c
 * @author Jacob C Smith
 */
// TODO: Improve documentation

// Include
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <array/array.h>

// Possible elements
char *A_element   = "A",
     *B_element   = "B",
     *C_element   = "C",
     *D_element   = "D",
     *X_element   = "X";

// Expected results
char  *_elements     [] = { 0x0 };
char  *A_elements    [] = { "A", 0x0 };
char  *B_elements    [] = { "B", 0x0 };
char  *C_elements    [] = { "C", 0x0 };
char  *AB_elements   [] = { "A", "B", 0x0 };
char  *BC_elements   [] = { "B", "C", 0x0 };
char  *AC_elements   [] = { "A", "C", 0x0 };
char  *ABC_elements  [] = { "A", "B", "C", 0x0 };

// Test results
enum result_e {
    zero,
    one,
    match
};

typedef enum result_e result_t;

int total_tests      = 0,
    total_passes     = 0,
    total_fails      = 0,
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;


// Forward declarations
int print_time_pretty        ( double seconds );
int run_tests                ();
int print_final_summary      ();
int print_test               ( const char  *scenario_name, const char    *test_name, bool passed );

bool test_add                ( int (*array_constructor)(array **), void  *value          , result_t expected );
bool test_get                ( int (*array_constructor)(array **), void **expected_values, result_t expected );

int test_empty_array         ( int (*array_constructor)(array **), char  *name );
int test_one_element_array   ( int (*array_constructor)(array **), char  *name, void **values );
int test_two_element_array   ( int (*array_constructor)(array **), char  *name, void **values );
int test_three_element_array ( int (*array_constructor)(array **), char  *name, void **values );

int construct_empty                     ( array **pp_array );
int construct_empty_addA_A              ( array **pp_array );
int construct_A_addB_AB                 ( array **pp_array ); 
int construct_AB_addC_ABC               ( array **pp_array );
int construct_ABC_clear_empty           ( array **pp_array );
int construct_AB_clear_empty            ( array **pp_array );
int construct_A_clear_empty             ( array **pp_array );
int construct_empty_fromelementsABC_ABC ( array **pp_array );
int construct_empty_fromelementsAB_AB   ( array **pp_array );
int construct_empty_fromelementsA_A     ( array **pp_array );

// Entry point
int main ( int argc, const char* argv[] )
{
    
    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Initialize the timer library
    timer_init();

    // Formatting
    printf("|==============|\n| ARRAY TESTER |\n|==============|\n\n");

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    printf("array took ");
    print_time_pretty ( (double)(t1-t0)/(double)timer_seconds_divisor() );
    printf(" to test\n");

    // Flush stdio
    fflush(stdout);

    // Exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int print_time_pretty ( double seconds )
{

    // Initialized data
    double _seconds     = seconds;
    size_t days         = 0,
           hours        = 0,
           minutes      = 0,
           __seconds    = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // Hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // Minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // Seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // Microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // Print days
    if ( days ) 
        printf("%d D, ", days);
    
    // Print hours
    if ( hours )
        printf("%d h, ", hours);

    // Print minutes
    if ( minutes )
        printf("%d m, ", minutes);

    // Print seconds
    if ( __seconds )
        printf("%d s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds )
        printf("%d ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds )
        printf("%d us", microseconds);
    
    // Success
    return 1;
}

int run_tests ( )
{
    // ... -> []
    test_empty_array(construct_empty, "empty");


    // [] -> add(A) -> [A]
    test_one_element_array(construct_empty_addA_A, "empty_addA_A", (void **)A_elements);

    // [A] -> add(B) -> [A, B]
    test_two_element_array(construct_A_addB_AB, "A_addB_AB", (void **)AB_elements);

    // [A] -> add(C) -> [A, B, C]
    test_three_element_array(construct_AB_addC_ABC, "AB_addC_ABC", (void **)ABC_elements);

    // [A, B, C] -> 

    // [A, B, C] -> clear() -> []
    test_empty_array(construct_ABC_clear_empty, "ABC_clear_empty");
 
    // [A, B] -> clear() -> []
    test_empty_array(construct_AB_clear_empty, "AB_clear_empty");

    // [A] -> clear() -> []
    test_empty_array(construct_A_clear_empty, "A_clear_empty");

    // Success
    return 1;
}
int print_final_summary ( )
{

    // Accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;

    // Print
    printf("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    printf("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));
 
    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // Success
    return 1;
}
int print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // Initialized data
    printf("%s_test_%-17s %s\n",scenario_name, test_name, (passed) ? "PASS" : "FAIL");

    // Increment the counters
    {
        if (passed)
        {
            ephemeral_passes++;
        }
        else
        {
            ephemeral_fails++;
        }

        ephemeral_tests++;
    }

    // Success
    return 1;
}

bool test_add ( int(*array_constructor)(array **pp_array), void  *value          , result_t expected )
{

    // Initialized data
    result_t  result = 0;
    array     *p_array = 0;

    // Build the array
    array_constructor(&p_array);

    result = array_add(p_array, value);

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}
bool test_get ( int(*array_constructor)(array **pp_array), void **expected_values, result_t expected )
{

    // Initialized data
    result_t  result       = 0;
    array    *p_array       = 0;
    void     *result_value = 0;

    // Build the array
    array_constructor(&p_array);

    // Get a key 
    //result_value = array_get(p_array, key);

    if (result_value == expected_values)
        result = match;
    else
        result = zero;

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}
bool test_get_count ( int(*array_constructor)(array **pp_array), size_t expected_size, result_t expected )
{
    // Initialized data
    result_t  result       = 0;
    array    *p_array      = 0;
    size_t    result_value = 0;

    // Build the array
    array_constructor(&p_array);

    // Get the size of the array
    array_get(p_array, 0, &result_value);

    if (result_value == expected_size)
        result = match;
    else
        result = zero;

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}

bool test_size ( int(*array_constructor)(array **pp_array), size_t expected_size, result_t expected )
{
    // Initialized data
    result_t  result       = 0;
    array    *p_array      = 0;

    // Build the array
    array_constructor(&p_array);

    // Get the size of the array

    if ( array_size(p_array) == expected_size )
        result = match;
    else
        result = zero;

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}

bool test_index ( int(*array_constructor)(array **pp_array), signed idx, void *expected_value, result_t expected )
{
    // Initialized data
    result_t  result       = 0;
    array    *p_array      = 0;
    void     *result_value = 0;

    // Build the array
    array_constructor(&p_array);

    // Index the array
    result = array_index(p_array, idx, &result_value);
    
    if ( result == 1 )
        if ( result_value == expected_value )
            result = match;

    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}

bool test_slice ( int(*array_constructor)(array **pp_array), signed lower, signed upper, void **expected_value, result_t expected )
{
    // Initialized data
    result_t  result       = 0;
    array    *p_array      = 0;
    void     *result_values[] = { 0, 0, 0, 0, (void *) 0 };

    // Build the array
    array_constructor(&p_array);

    // Index the array
    result = array_slice(p_array, &result_values, lower, upper);
    
    // Error check
    if ( result == zero )
        goto done;

    result = match;

        for (size_t i = 0; i < upper-lower; i++)
        {
            if ( result_values[i] != expected_value[i] )
                result = zero;
        }
        

    done:
    // Free the array
    array_destroy(&p_array);

    // Return result
    return (result == expected);
}


int construct_empty ( array **pp_array )
{
    // Construct an array
    array_construct(pp_array, 1);

    // array = []
    return 1;
}
int construct_empty_addA_A ( array **pp_array )
{

    // Construct a [] array
    construct_empty(pp_array);

    // [] -> add(A) -> [A]
    array_add(*pp_array, A_element);
 
    // array = [A]
    // Success
    return 1;
}
int construct_A_addB_AB ( array **pp_array )
{

    // Construct an [A] array 
    construct_empty_addA_A(pp_array);

    // [A] -> add(B) -> [B]
    array_add(*pp_array , B_element);
 
    // array = [A, B]
    // Success
    return 1;
}
int construct_AB_addC_ABC ( array **pp_array )
{

    // Construct an [A, B] array
    construct_A_addB_AB(pp_array);

    // [A, B] -> add(C) -> [A, B, C]
    array_add(*pp_array, C_element);

    // array = [A, B, C]
    // Success
    return 1;
}
int construct_ABC_clear_empty ( array **pp_array )
{
    // Construct an [A, B, C] array
    construct_AB_addC_ABC(pp_array);

    // [A, B, C] -> clear() -> []
    array_clear(*pp_array);

    // array = []
    // Success
    return 1;
}
int construct_AB_clear_empty ( array **pp_array )
{
    // Construct an [A, B] array
    construct_A_addB_AB(pp_array);

    // [A, B] -> clear() -> []
    array_clear(*pp_array);

    // array = []
    // Success
    return 1;
}
int construct_A_clear_empty ( array **pp_array )
{

    // Construct an [A] array
    construct_empty_addA_A(pp_array);

    // [A] -> clear() -> []
    array_clear(*pp_array);

    // array = []
    // Success
    return 1;
}
int construct_empty_fromelementsABC_ABC ( array **pp_array )
{

    array_from_elements(pp_array, (void **)ABC_elements);

    // Success
    return 1;
}
int construct_empty_fromelementsAB_AB ( array **pp_array )
{

    array_from_elements(pp_array, (void **)AB_elements);

    // Success
    return 1;
}
int construct_empty_fromelementsA_A ( array **pp_array )
{

    array_from_elements(pp_array, (void **)A_elements);

    // Success
    return 1;
}

int test_empty_array ( int (*array_constructor)(array **pp_array), char *name)
{

    printf("Scenario: %s\n", name);

    print_test(name, "array_add_A"    , test_add(array_constructor, A_element, one) );
    print_test(name, "array_add_B"    , test_add(array_constructor, B_element, one) );
    print_test(name, "array_index0"   , test_index(array_constructor, 0, (void *)0, zero) );
    print_test(name, "array_index1"   , test_index(array_constructor, 1, (void *)0, zero) );
    print_test(name, "array_get"      , test_get(array_constructor, (void **)_elements, zero) );    
    print_test(name, "array_get_count", test_get_count(array_constructor, 0, match) );

    print_final_summary();

    return 1;
}
int test_one_element_array ( int (*array_constructor)(array **pp_array), char *name, void **values )
{

    printf("SCENARIO: %s\n", name);

    print_test(name, "array_add_D"    , test_add(array_constructor, D_element, one) );
    print_test(name, "array_get"      , test_get(array_constructor, values, zero) );
    print_test(name, "array_size"     , test_size(array_constructor, 1, match));
    print_test(name, "array_index0"   , test_index(array_constructor, 0, values[0], match) );
    print_test(name, "array_index1"   , test_index(array_constructor, 1, (void *)0, zero) );
    print_test(name, "array_get"      , test_get(array_constructor, values, zero) );    
    print_test(name, "array_get_count", test_get_count(array_constructor, 1, match) );
    
    print_final_summary();
    
    // Success
    return 1;
}
int test_two_element_array ( int (*array_constructor)(array **pp_array), char *name, void **values )
{
    printf("SCENARIO: %s\n", name);

    print_test(name, "array_add_D"    , test_add(array_constructor, D_element, one) );
    print_test(name, "array_get"      , test_get(array_constructor, values, zero) );
    print_test(name, "array_size"     , test_size(array_constructor, 2, match));
    print_test(name, "array_index0"   , test_index(array_constructor, 0, values[0], match) );
    print_test(name, "array_index1"   , test_index(array_constructor, 1, values[1], match) );  
    print_test(name, "array_index2"   , test_index(array_constructor, 2, (void *)0, zero) );  
    print_test(name, "array_get_count", test_get_count(array_constructor, 2, match) );
    
    print_final_summary();
    
    // Success
    return 1;
}
int test_three_element_array ( int (*array_constructor)(array **pp_array), char *name, void **values )
{
    printf("SCENARIO: %s\n", name);

    print_test(name, "array_add_D"    , test_add(array_constructor, D_element, one) );
    print_test(name, "array_get"      , test_get(array_constructor, values, zero) );
    print_test(name, "array_size"     , test_size(array_constructor, 3, match));
    print_test(name, "array_index0"   , test_index(array_constructor, 0, values[0], match) );
    print_test(name, "array_index1"   , test_index(array_constructor, 1, values[1], match) );  
    print_test(name, "array_index2"   , test_index(array_constructor, 2, values[2], match) );  
    print_test(name, "array_index3"   , test_index(array_constructor, 3, (void *)0, zero) );  
    print_test(name, "array_get_count", test_get_count(array_constructor, 3, match) );
    
    print_final_summary();
    
    // Success
    return 1;
}
