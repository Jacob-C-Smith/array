cmake_minimum_required (VERSION 3.26)

project ("array")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")
if ( NOT "${HAS_ARRAY}")
    message("[array] Adding array project")
    set(HAS_ARRAY true PARENT_SCOPE)
endif()

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Add source to this project's executable.
add_executable (array_example "main.c" "array.c")
target_include_directories(array_example PUBLIC "include")

# Add source to the tester
add_executable (array_test "array_test.c" "array.c")
target_include_directories(array_test PUBLIC "include")

# Add source to this project's library
add_library (array SHARED "array.c")
target_include_directories(array PUBLIC include include/array)
