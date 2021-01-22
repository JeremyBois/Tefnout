cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
PROJECT("Tefnout")




# ------------------------------------------------------------------------------
# We need a CMAKE_DIR with some code to find external dependencies
# ------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules/")




# ------------------------------------------------------------------------------
# Provide default for non multi-configuration generator (like Visual Studio)
# ------------------------------------------------------------------------------
# CMAKE_CONFIGURATION_TYPES used only for multi-configuration
# CMAKE_BUILD used only for single configuration
if (NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
          message(VERBOSE "*** Setting build type to 'Debug' because none was specified.")
          set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
          # Set the possible values of build type for cmake-gui
          set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "Distribution")
    endif()

    # Use clang if None exists
    if(NOT CMAKE_C_COMPILER)
          message(VERBOSE "*** Setting C compiler to 'clang' because none was specified.")
          set(CMAKE_C_COMPILER clang CACHE STRING "Choose the compiler." FORCE)
          # Set the possible values of build type for cmake-gui
          set_property(CACHE CMAKE_C_COMPILER PROPERTY STRINGS "clang" "gcc" "MSVC")
    endif()

    # Use clang++ if None exists
    if(NOT CMAKE_CXX_COMPILER)
          message(VERBOSE "*** Setting C++ compiler to 'clang++' because none was specified.")
          set(CMAKE_CXX_COMPILER clang++ CACHE STRING "Choose the compiler." FORCE)
          # Set the possible values of build type for cmake-gui
          set_property(CACHE CMAKE_CXX_COMPILER PROPERTY STRINGS "clang++" "gcc++" "MSVC")
    endif()

    message(STATUS "${PROJECT_NAME} build as ${CMAKE_BUILD_TYPE}.")
    message(STATUS "C++ compiler is ${CMAKE_CXX_COMPILER} and C compiler is ${CMAKE_C_COMPILER}.")
else()
    # Display information about the build parameters
    message(STATUS "${PROJECT_NAME} will be build for multi-configuration generator (${CMAKE_CONFIGURATION_TYPES}")
endif()





# ------------------------------------------------------------------------------
# Find - GLM
# ------------------------------------------------------------------------------
# Header only
set(Tefnout_GLM_ROOT_DIR ${PROJECT_SOURCE_DIR}/vendors/glm
    CACHE STRING "Path used to search first for GLM header files." FORCE)
set(GLM_ROOT_DIR ${Tefnout_GLM_ROOT_DIR})

find_package(GLM REQUIRED)
if(NOT GLM_FOUND)
    message(FATAL_ERROR "GLM library not found - Required for math")
else()
    message(STATUS "GLM included at ${GLM_INCLUDE_DIRS}")
endif()




# ------------------------------------------------------------------------------
# Find - SPDLOG
# ------------------------------------------------------------------------------
# Precompiled library (local)
# https://github.com/gabime/spdlog/issues/1286
set(Tefnout_SPDLOG_ROOT_DIR ${PROJECT_SOURCE_DIR}/vendors/spdlog/build/install
    CACHE FILEPATH "Path of SPDLOG installation directory." FORCE)
set(spdlog_DIR ${Tefnout_SPDLOG_ROOT_DIR}/lib/cmake/spdlog)
find_package(spdlog REQUIRED)
message(STATUS "SPDLOG included at ${spdlog_DIR}")




# ------------------------------------------------------------------------------
# Create a static or shared library target.
# ------------------------------------------------------------------------------
# Allow the developer to select if Dynamic or Static libraries are built
OPTION (Tefnout_BUILD_SHARED_LIBS "Build Shared Libraries" ON)
if (Tefnout_BUILD_SHARED_LIBS)
    set (Tefnout_LIB_NAME tefnout-shared)
    set (LIB_TYPE SHARED)  # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
else()
    set (Tefnout_LIB_NAME tefnout-static)
    set (LIB_TYPE STATIC)  # Set the LIB_TYPE variable to STATIC
endif ()

# Display information about the build parameters
message(STATUS "${Tefnout_LIB_NAME} will be build as ${LIB_TYPE} library (${CMAKE_BUILD_TYPE})")


file(
    GLOB
    Tefnout_SOURCES
    src/*
    src/Tefnout/Core/*
)
add_library(${Tefnout_LIB_NAME} ${LIB_TYPE} ${Tefnout_SOURCES})

# See https://cmake.org/pipermail/cmake/2016-May/063400.html
if (Tefnout_BUILD_SHARED_LIBS)
    # Shared library
    target_include_directories(${Tefnout_LIB_NAME}
        PRIVATE BEFORE
            ${PROJECT_SOURCE_DIR}/src
        PRIVATE
            ${GLM_INCLUDE_DIRS}
    )
    # Set pre processor command for Shared lib export
    target_compile_definitions(${Tefnout_LIB_NAME}
    PRIVATE
        TEFNOUT_SHARED="USED"
        TEFNOUT_SHARED_EXPORTS="USED"
    )
else()
    # Static library
    target_include_directories(${Tefnout_LIB_NAME}
        PUBLIC BEFORE
            ${PROJECT_SOURCE_DIR}/src
        PUBLIC
            ${GLM_INCLUDE_DIRS}
    )
endif()

# Expose SPDLOG as a PUBLIC dependency
# because it is used in both header and source files
target_link_libraries(${Tefnout_LIB_NAME}
    PUBLIC
        spdlog::spdlog
)
set_target_properties(${Tefnout_LIB_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)

# Read only alias
add_library(tefnout ALIAS ${Tefnout_LIB_NAME})




# ------------------------------------------------------------------------------
# Build options based on platform (Windows, Unix)
# ------------------------------------------------------------------------------
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(${Tefnout_LIB_NAME}
        PUBLIC
            /W4
            # /WX
    )
    target_link_options(${MY_LIB_NAME}
        PUBLIC
            -MACHINE:x64
    )
else()
    # lots of warnings and all warnings as errors
    target_compile_options(${Tefnout_LIB_NAME}
        PUBLIC
            -Wall -Wextra -pedantic
            -m64
            # -Werror
    )
endif()




# ------------------------------------------------------------------------------
# Build options based on build type (Debug, Release, Distribution)
# ------------------------------------------------------------------------------
if (NOT CMAKE_CONFIGURATION_TYPES)  # Only for generator without multi-configurations
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(TEFNOUT_LOG_LEVEL 1)
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(TEFNOUT_LOG_LEVEL 3)
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "Distribution")
        set(TEFNOUT_LOG_LEVEL 4)
    endif()

    message(STATUS "${Tefnout_LIB_NAME} logging level set to ${TEFNOUT_LOG_LEVEL} ${CMAKE_CPP_FLAGS}")

    target_compile_definitions(${Tefnout_LIB_NAME}
        PRIVATE
            TEFNOUT_LOG_LEVEL=${TEFNOUT_LOG_LEVEL}
    )
endif()



# ------------------------------------------------------------------------------
# Add a list of header files to precompile.
# https://cmake.org/cmake/help/latest/command/target_precompile_headers.html
# ------------------------------------------------------------------------------
target_precompile_headers(${Tefnout_LIB_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/TefnoutPCH.hpp
)
