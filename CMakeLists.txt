cmake_minimum_required (VERSION 3.8)

project ("array")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Add source to this project's executable.
add_executable (array_example)
target_include_directories(array_example PUBLIC "include")
target_sources(array_example PRIVATE  "array.c" "main.c")

# Add source to the tester
add_executable (array_test)
target_include_directories(array_test PUBLIC "include")
target_sources(array_test PRIVATE "array.c" "array_test.c")

# Add source to this project's library
add_library    (array SHARED "array.c")
target_include_directories(array PUBLIC "include")

# TODO: Add tests and install targets if needed.
