#
# Find GLFW
#
# Try to find GLFW : OpenGL Mathematics.
# This module defines
#
#       GLFW_INCLUDE_DIRS
#       GLFW_LIBRARIES
#       GLFW_FOUND
#
# and the following target (not glfw::glfw to be compatible with GLFW cmake project)
#
#       glfw
#
#
# The following variables can be set as arguments for the module to search in a specific
# location first
# - GLFW_ROOT_DIR : Root library directory of GLFW


# Additional modules
include(FindPackageHandleStandardArgs)


# Get lib version from header
function(ParseGLFWVersion FILENAME VARNAME)
        set(PATTERN "^#define ${VARNAME}.*$")
        file(STRINGS "${GLFW_INCLUDE_DIR}/${FILENAME}" TMP REGEX ${PATTERN})
        string(REGEX MATCHALL "[0-9]+" TMP ${TMP})
        set(${VARNAME} ${TMP} PARENT_SCOPE)
endfunction()


if (WIN32)
    # Look after headers
    find_path(GLFW_INCLUDE_DIR
        NAMES
            GLFW/glfw3.h
        HINTS
            "${GLFW_ROOT_DIR}"
            "${GLFW_ROOT_DIR}/include"
        PATHS
            "$ENV{PROGRAMFILES}/GLFW/include"
        DOC
            "The GLFW3 includes directory"
        )
    # Look after lib
    find_library(GLFW_LIBRARY
            NAMES
                glfw3
            HINTS
                "${GLFW_ROOT_DIR}"
                "${GLFW_ROOT_DIR}/lib"
            PATHS
                "$ENV{PROGRAMFILES}/GLFW/lib"
            DOC
                "The GLFW3 library directory"
        )
    # Handle REQUIRED argument, define *_FOUND variable
    find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_INCLUDE_DIR GLFW_LIBRARY)
else()
    message(VERBOSE "Checking for Linux required files for GLFW.")
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)
    if(NOT X11_Xrandr_FOUND)
        message(FATAL_ERROR "Xrandr library not found - required for GLFW")
    endif()

    if(NOT X11_xf86vmode_FOUND)
        message(FATAL_ERROR "xf86vmode library not found - required for GLFW")
    endif()

    if(NOT X11_Xcursor_FOUND)
        message(FATAL_ERROR "Xcursor library not found - required for GLFW")
    endif()

    if(NOT X11_Xinerama_FOUND)
        message(FATAL_ERROR "Xinerama library not found - required for GLFW")
    endif()

    if(NOT X11_Xi_FOUND)
        message(FATAL_ERROR "Xi library not found - required for GLFW")
    endif()

    # All needed by GLFW
    list(APPEND GLFW_X11_LIBRARY "${X11_Xrandr_LIB}" "${X11_Xxf86vm_LIB}" "${X11_Xcursor_LIB}"
                                 "${X11_Xinerama_LIB}" "${X11_Xi_LIB}" "${X11_LIBRARIES}")
    set(GLFW_COMPILE_OPTION "${CMAKE_THREAD_LIBS_INIT}" -lrt -ldl)

    # Look after headers
    find_path(GLFW_INCLUDE_DIR
        NAMES
            GLFW/glfw3.h
        HINTS
            "${GLFW_ROOT_DIR}"
            "${GLFW_ROOT_DIR}/include"
        PATHS
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
        DOC
            "The GLFW3 includes directory"
        )
    # Look after lib
    find_library(GLFW_LIBRARY
            NAMES
                glfw3 glfw libglfw
            HINTS
                "${GLFW_ROOT_DIR}"
                "${GLFW_ROOT_DIR}/lib"
            PATHS
                /usr/lib
                /usr/local/lib
                /sw/lib
                /opt/local/lib
            DOC
                "The GLFW3 library directory"
        )

    # Handle REQUIRED argument, define *_FOUND variable
    find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_INCLUDE_DIR GLFW_LIBRARY)

    if(NOT GLFW_FOUND)
        find_package(PkgConfig REQUIRED)
        pkg_search_module(GLFW REQUIRED glfw3)
    endif()
    if(NOT GLFW_FOUND)
        find_package(glfw3 3.3 REQUIRED)
    endif()
endif()


if(NOT GLFW_FOUND)
    message(FATAL_ERROR "GLFW3 library not found")
else()
    # Populate versions for GLFW
    if(EXISTS "${GLFW_INCLUDE_DIR}/GLFW/glfw3.h")
        ParseGLFWVersion(GLFW/glfw3.h GLFW_VERSION_MAJOR)
        ParseGLFWVersion(GLFW/glfw3.h GLFW_VERSION_MINOR)
        ParseGLFWVersion(GLFW/glfw3.h GLFW_VERSION_REVISION)
        set(GLFW_VERSION "${GLFW_VERSION_MAJOR}.${GLFW_VERSION_MINOR}.${GLFW_VERSION_REVISION}")
    endif()
endif()

# Build target
if(GLFW_FOUND AND NOT TARGET glfw)
    set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
    set(GLFW_LIBRARIES ${GLFW_LIBRARY} ${GLFW_X11_LIBRARY} ${GLFW_COMPILE_OPTION})

    # Prepare target
    add_library(glfw SHARED IMPORTED)

    set_target_properties(glfw PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
        IMPORTED_LOCATION "${GLFW_LIBRARY}"
    )

    target_link_libraries(glfw
        INTERFACE
            ${GLFW_X11_LIBRARY}
    )

    target_compile_options(glfw
        INTERFACE
            ${GLFW_COMPILE_OPTION}
    )
endif()


# Hide internal variables
mark_as_advanced(GLFW_INCLUDE_DIR GLFW_LIBRARY GLFW_FOUND)
mark_as_advanced(GLFW_VERSION_MAJOR GLFW_VERSION_MINOR GLFW_VERSION_REVISION)
