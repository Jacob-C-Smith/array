# Need CMake 3.20
cmake_minimum_required (VERSION 3.20)

# This is the name of the repository
project ("array")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set for debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Find the sync module
if ( NOT "${HAS_SYNC}")

    # Log
    message("[array] Adding sync project")

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    set(HAS_SYNC true PARENT_SCOPE)
endif()

# Add the array project
if ( NOT "${HAS_ARRAY}")

    # Log
    message("[array] Adding array project")

    # Let any parent project know that it has the array project
    set(HAS_ARRAY true PARENT_SCOPE)
endif()

# Add source to this project's executable.
add_executable (array_example "main.c" "array.c")
add_dependencies(array_example sync)
target_include_directories(array_example PUBLIC include include/array ${CMAKE_SOURCE_DIR}/extern/sync/include/)
target_link_libraries(array_example sync)

# Add source to the tester
add_executable (array_test "array_test.c" "array.c")
add_dependencies(array_test sync)
target_include_directories(array_test PUBLIC include)
target_link_libraries(array_test sync)

# Add source to this project's library
add_library (array SHARED "array.c")
add_dependencies(array sync)
target_include_directories(array PUBLIC include include/array ${CMAKE_SOURCE_DIR}/extern/sync/include/)
target_link_libraries(array sync)