# ARRAY CMake configuration file:
# This file is meant to be placed in a cmake subfolder of ARRAY-devel-2.x.y-VC

cmake_minimum_required(VERSION 3.0)

include(FeatureSummary)
set_package_properties(ARRAY PROPERTIES
    URL "https://g10.app/status/#abstract_data_i"
    DESCRIPTION "Array ADT"
)

# Copied from `configure_package_config_file`
macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

# Copied from `configure_package_config_file`
macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

set(ARRAY_FOUND TRUE)

# For compatibility with autotools ARRAY-config.cmake, provide ARRAY_* variables.

set_and_check(ARRAY_PREFIX       "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(ARRAY_EXEC_PREFIX  "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(ARRAY_INCLUDE_DIR  "${ARRAY_PREFIX}/include")
set(ARRAY_INCLUDE_DIRS           "${ARRAY_INCLUDE_DIR}")
set_and_check(ARRAY_BINDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")
set_and_check(ARRAY_LIBDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")

set(ARRAY_LIBRARIES ARRAY::ARRAY)

# All targets are created, even when some might not be requested though COMPONENTS.
# This is done for compatibility with CMake generated ARRAY-target.cmake files.

set(_ARRAY_library     "${ARRAY_LIBDIR}/array.lib")
set(_ARRAY_dll_library "${ARRAY_BINDIR}/array.dll")
if(EXISTS "${_ARRAY_library}" AND EXISTS "${_ARRAY_dll_library}")
    if(NOT TARGET ARRAY::ARRAY)
        add_library(ARRAY::ARRAY SHARED IMPORTED)
        set_target_properties(ARRAY::ARRAY
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${ARRAY_INCLUDE_DIRS}"
                IMPORTED_IMPLIB "${_ARRAY_library}"
                IMPORTED_LOCATION "${_ARRAY_dll_library}"
                COMPATIBLE_INTERFACE_BOOL "ARRAY_SHARED"
                INTERFACE_ARRAY_SHARED "ON"
        )
    endif()
    set(ARRAY_ARRAY_FOUND TRUE)
else()
    set(ARRAY_ARRAY_FOUND FALSE)
endif()
unset(_ARRAY_library)
unset(_ARRAY_dll_library)

check_required_components(ARRAY)
