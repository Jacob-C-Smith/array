cmake_minimum_required (VERSION 3.8)

project ("array")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Add source to this project's executable.
add_executable (array_example "main.c")
target_include_directories(array_example PUBLIC "include")
target_link_libraries(array_example PUBLIC array)

# Add source to the tester
add_executable (array_test "array_test.c")
target_include_directories(array_test PUBLIC "include")
target_link_libraries(array_test PUBLIC array)

# Add source to this project's library
add_library (array "array.c")
target_include_directories(array PUBLIC "include")

# TODO: Add tests and install targets if needed.
