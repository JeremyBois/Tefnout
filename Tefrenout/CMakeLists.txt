cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
PROJECT("Tefrenout")


# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules/")


# ------------------------------------------------------------------------------
# Provide default for non multi-configuration generator (like Visual Studio)
# ------------------------------------------------------------------------------
# CMAKE_CONFIGURATION_TYPES used only for multi-configuration
# CMAKE_BUILD used only for single configuration
if (NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
          message("*** Setting build type to 'Debug' because none was specified.")
          set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
          # Set the possible values of build type for cmake-gui
          set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
    endif()

    # Use clang if None exists
    if(NOT CMAKE_C_COMPILER)
          message("*** Setting C compiler to 'clang' because none was specified.")
          set(CMAKE_C_COMPILER clang CACHE STRING "Choose the compiler." FORCE)
          # Set the possible values of build type for cmake-gui
          set_property(CACHE CMAKE_C_COMPILER PROPERTY STRINGS "clang" "gcc" "MSVC")
    endif()

    # Use clang++ if None exists
    if(NOT CMAKE_CXX_COMPILER)
          message("*** Setting C++ compiler to 'clang++' because none was specified.")
          set(CMAKE_CXX_COMPILER clang++ CACHE STRING "Choose the compiler." FORCE)
          # Set the possible values of build type for cmake-gui
          set_property(CACHE CMAKE_CXX_COMPILER PROPERTY STRINGS "clang++" "gcc++" "MSVC")
    endif()

    message("*** ${PROJECT_NAME} build as ${CMAKE_BUILD_TYPE} for architecture ${CMAKE_GENERATOR_PLATFORM}.")
    message("*** C++ compiler is ${CMAKE_CXX_COMPILER} and C compiler is ${CMAKE_C_COMPILER}.")
else()
    # Display information about the build parameters
    message("*** ${PROJECT_NAME} will be build for multi-configuration generator (${CMAKE_CONFIGURATION_TYPES}")
endif()


#
# Find GLM
#
set(GLM_ROOT_DIR ${PROJECT_SOURCE_DIR}/vendors/glm)
find_package(GLM REQUIRED)
if(NOT GLM_FOUND)
    message(FATAL_ERROR "GLM library not found - Required for math")
else()
    message(STATUS "GLM included at ${GLM_INCLUDE_DIRS}")
endif()

#
# Find SPDLOG
#
add_subdirectory(${PROJECT_SOURCE_DIR}/vendors/spdlog)
find_package(spdlog REQUIRED)


# ------------------------------------------------------------------------------
# Create a static and shared library target.
# ------------------------------------------------------------------------------
# Allow the developer to select if Dynamic or Static libraries are built
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" ON)
if (BUILD_SHARED_LIBS)
    set (MY_LIB_NAME tefrenout-shared)
    set (LIB_TYPE SHARED)  # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
else()
    set (MY_LIB_NAME tefrenout-static)
    set (LIB_TYPE STATIC)  # Set the LIB_TYPE variable to STATIC
endif (BUILD_SHARED_LIBS)

# Display information about the build parameters
message("*** ${MY_LIB_NAME} will be build as ${LIB_TYPE} library (${CMAKE_BUILD_TYPE})")


file(
    GLOB
    library_SOURCES
    src/*
    src/Tefrenout/Core/*
)

add_library(${MY_LIB_NAME} ${LIB_TYPE} ${library_SOURCES})

# See https://cmake.org/pipermail/cmake/2016-May/063400.html
if (BUILD_SHARED_LIBS)
    # Shared library
    target_include_directories(${MY_LIB_NAME}
        PRIVATE BEFORE
            ${PROJECT_SOURCE_DIR}/src
        PRIVATE
            ${GLM_INCLUDE_DIRS}
    )
    # Set pre processor command for Shared lib export
    target_compile_definitions(${MY_LIB_NAME}
    PRIVATE
        TEFRENOUT_SHARED="USED"
        TEFRENOUT_SHARED_EXPORTS="USED"
    )
else()
    # Static library
    target_include_directories(${MY_LIB_NAME}
        PUBLIC BEFORE
            ${PROJECT_SOURCE_DIR}/src
        PUBLIC
            ${GLM_INCLUDE_DIRS}
    )
endif()

# Get
target_link_libraries(${MY_LIB_NAME}
    PRIVATE
        spdlog::spdlog
)

set_target_properties(${MY_LIB_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)

# More warnings
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(${MY_LIB_NAME}
        PUBLIC /W4
        # PUBLIC /WX
    )
else()
    # lots of warnings and all warnings as errors
    target_compile_options(${MY_LIB_NAME}
        PUBLIC -Wall -Wextra -pedantic
        # PUBLIC -Werror
    )
endif()

# Read only alias
add_library(tefrenout ALIAS ${MY_LIB_NAME})
