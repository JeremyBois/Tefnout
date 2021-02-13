#
# Find STB_IMAGE
# https://github.com/nothings/stb/blob/master/stb_image.h
#
# Try to find STB_IMAGE : Image / Texture loader lib
# This module defines
#
#       STB_IMAGE_INCLUDE_DIRS
#       STB_IMAGE_SOURCES
#       STB_IMAGE_FOUND
#
# and the following target
#
#       stb_image
#
#
# The following variables can be set as arguments for the module to search in a specific
# location first
# - STB_IMAGE_ROOT_DIR : Root library directory of STB_IMAGE



# Additional modules
include(FindPackageHandleStandardArgs)

if (WIN32)
    # Find include files for Windows
    find_path(STB_IMAGE_INCLUDE_DIR
        NAMES
            stb_image/stb_image.h
        HINTS
            ${STB_IMAGE_ROOT_DIR}
            ${STB_IMAGE_ROOT_DIR}/include
        PATHS
            $ENV{PROGRAMFILES}/include
        DOC "The directory where stb_image/stb_image.h resides")
else()
    # Find include files for UNIX
    find_path(STB_IMAGE_INCLUDE_DIR
        NAMES
            stb_image/stb_image.h
        HINTS
            ${STB_IMAGE_ROOT_DIR}
            ${STB_IMAGE_ROOT_DIR}/include
        PATHS
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
        DOC "The directory where stb_image/stb_image.h resides")
endif()

# Handle REQUIRED argument, define *_FOUND variable
find_package_handle_standard_args(STB_IMAGE DEFAULT_MSG STB_IMAGE_INCLUDE_DIR)

# Define STB_IMAGE_INCLUDE_DIRS
if (STB_IMAGE_FOUND AND NOT TARGET stb_image)
    set(STB_IMAGE_INCLUDE_DIRS ${STB_IMAGE_INCLUDE_DIR})
    set(STB_IMAGE_SOURCES "${STB_IMAGE_INCLUDE_DIR}/stb_image/stb_image")

    # Prepare target
    add_library(stb_image SHARED ${STB_IMAGE_SOURCES})

    set_target_properties(stb_image PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${STB_IMAGE_INCLUDE_DIR}"
    )
endif()

# Hide internal variables
mark_as_advanced(STB_IMAGE_INCLUDE_DIR STB_IMAGE_FOUND)
