#
# Find GLM
#
# Try to find GLM : OpenGL Mathematics.
# This module defines
#
#       GLM_INCLUDE_DIRS
#       GLM_FOUND
#
# and the following target
#
#       glm::glm
#
#
# The following variables can be set as arguments for the module to search in a specific
# location first
# - GLM_ROOT_DIR : Root library directory of GLM
#
# References:
# - https://github.com/Hopson97/MineCraft-One-Week-Challenge/blob/master/cmake_modules/FindGLM.cmake
# - https://github.com/Groovounet/glm/blob/master/util/FindGLM.cmake
# - https://github.com/Shot511/OpenGLSampleCmake/blob/master/cmake/FindGLM.cmake
#

# Additional modules
include(FindPackageHandleStandardArgs)

if (WIN32)
    # Find include files for Windows
    find_path(GLM_INCLUDE_DIR
        NAMES
            glm/glm.hpp
        HINTS
            ${GLM_ROOT_DIR}
            ${GLM_ROOT_DIR}/include
        PATHS
            $ENV{PROGRAMFILES}/include
        DOC "The directory where glm/glm.hpp resides")
else()
    # Find include files for UNIX
    find_path(GLM_INCLUDE_DIR
        NAMES
            glm/glm.hpp
        HINTS
            ${GLM_ROOT_DIR}
            ${GLM_ROOT_DIR}/include
        PATHS
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
        DOC "The directory where glm/glm.hpp resides")
endif()

# Handle REQUIRED argument, define *_FOUND variable
find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)

# Define GLM_INCLUDE_DIRS
if (GLM_FOUND AND NOT TARGET glm::glm)
    set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})

    # Prepare target
    add_library(glm::glm INTERFACE IMPORTED)

    set_target_properties(glm::glm PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${GLM_INCLUDE_DIR}"
    )
endif()

# Hide internal variables
mark_as_advanced(GLM_INCLUDE_DIR GLM_FOUND)
