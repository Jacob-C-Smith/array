set(PACKAGE_VERSION "1.0.0")

if(PACKAGE_FIND_VERSION_RANGE)
    # Package version must be in the requested version range
    if ((PACKAGE_FIND_VERSION_RANGE_MIN STREQUAL "INCLUDE" AND PACKAGE_VERSION VERSION_LESS PACKAGE_FIND_VERSION_MIN)
        OR ((PACKAGE_FIND_VERSION_RANGE_MAX STREQUAL "INCLUDE" AND PACKAGE_VERSION VERSION_GREATER PACKAGE_FIND_VERSION_MAX)
        OR (PACKAGE_FIND_VERSION_RANGE_MAX STREQUAL "EXCLUDE" AND PACKAGE_VERSION VERSION_GREATER_EQUAL PACKAGE_FIND_VERSION_MAX)))
        set(PACKAGE_VERSION_COMPATIBLE FALSE)
    else()
        set(PACKAGE_VERSION_COMPATIBLE TRUE)
    endif()
else()
    if(PACKAGE_VERSION VERSION_LESS PACKAGE_FIND_VERSION)
        set(PACKAGE_VERSION_COMPATIBLE FALSE)
    else()
        set(PACKAGE_VERSION_COMPATIBLE TRUE)
        if(PACKAGE_FIND_VERSION STREQUAL PACKAGE_VERSION)
            set(PACKAGE_VERSION_EXACT TRUE)
        endif()
    endif()
endif()

# if the using project doesn't have CMAKE_SIZEOF_VOID_P set, fail.
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "")
    set(PACKAGE_VERSION_UNSUITABLE TRUE)
endif()

# check that the installed version has the same 32/64bit-ness as the one which is currently searching:
if(NOT (CMAKE_SIZEOF_VOID_P STREQUAL "8" OR CMAKE_SIZEOF_VOID_P STREQUAL "4"))
    set(PACKAGE_VERSION "${PACKAGE_VERSION} (32+64bit)")
    set(PACKAGE_VERSION_UNSUITABLE TRUE)
endif()
