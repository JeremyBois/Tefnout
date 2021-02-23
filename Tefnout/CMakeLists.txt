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
          message(VERBOSE "Setting build type to 'Debug' because none was specified.")
          set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
          # Set the possible values of build type for cmake-gui
          set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "Distribution")
    endif()

    # Use clang if None exists
    if(NOT CMAKE_C_COMPILER)
          message(VERBOSE "Setting C compiler to 'clang' because none was specified.")
          set(CMAKE_C_COMPILER clang CACHE STRING "Choose the compiler." FORCE)
          # Set the possible values of build type for cmake-gui
          set_property(CACHE CMAKE_C_COMPILER PROPERTY STRINGS "clang" "gcc" "MSVC")
    endif()

    # Use clang++ if None exists
    if(NOT CMAKE_CXX_COMPILER)
          message(VERBOSE "Setting C++ compiler to 'clang++' because none was specified.")
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
# Options
# ------------------------------------------------------------------------------
# Allow the developer to select if Dynamic or Static libraries are built
OPTION (TEFNOUT_BUILD_SHARED_LIBS "Build Shared Libraries" ON)

# Allow the developer to select if Dynamic or Static libraries are built
OPTION (TEFNOUT_USE_PRECOMPILED_GLFW "Use pre-compiled libraries for GLFW" ON)



# ------------------------------------------------------------------------------
# Find - STB_IMAGE
# ------------------------------------------------------------------------------
# Header only
set(TEFNOUT_STB_IMAGE_ROOT_DIR ${PROJECT_SOURCE_DIR}/vendors/stb_image
    CACHE STRING "Path used to search first for STB_IMAGE header files." FORCE)
set(STB_IMAGE_ROOT_DIR ${TEFNOUT_STB_IMAGE_ROOT_DIR})
find_package(STB_IMAGE REQUIRED)

if(NOT STB_IMAGE_FOUND)
    message(FATAL_ERROR "STB_IMAGE library not found - Required for image loading")
else()
    message(STATUS "Found STB_IMAGE")
    message(STATUS "      headers: ${STB_IMAGE_INCLUDE_DIRS}")
endif()



# ------------------------------------------------------------------------------
# Find - GLM
# ------------------------------------------------------------------------------
# Header only
set(TEFNOUT_GLM_ROOT_DIR ${PROJECT_SOURCE_DIR}/vendors/glm
    CACHE STRING "Path used to search first for GLM header files." FORCE)
set(GLM_ROOT_DIR ${TEFNOUT_GLM_ROOT_DIR})
find_package(GLM REQUIRED)

if(NOT GLM_FOUND)
    message(FATAL_ERROR "GLM library not found - Required for math")
else()
    message(STATUS "Found GLM")
    message(STATUS "      headers: ${GLM_INCLUDE_DIRS}")
endif()



# ------------------------------------------------------------------------------
# Find - SPDLOG
# ------------------------------------------------------------------------------
# Precompiled library (local)
# https://github.com/gabime/spdlog/issues/1286
set(TEFNOUT_SPDLOG_ROOT_DIR "${PROJECT_SOURCE_DIR}/vendors/spdlog/build/install/${CMAKE_BUILD_TYPE}"
    CACHE FILEPATH "Path of SPDLOG installation directory." FORCE)
set(spdlog_DIR ${TEFNOUT_SPDLOG_ROOT_DIR}/lib/cmake/spdlog)
find_package(spdlog REQUIRED)

message(STATUS "SPDLOG included at ${spdlog_DIR}")



# ------------------------------------------------------------------------------
# Find - OPENGL
# ------------------------------------------------------------------------------
if (UNIX)
    if(NOT DEFINED OpenGL_GL_PREFERENCE)
        set(OpenGL_GL_PREFERENCE "GLVND")
    elseif(NOT OpenGL_GL_PREFERENCE MATCHES "^(GLVND|LEGACY)$")
          message(FATAL_ERROR
            "OpenGL_GL_PREFERENCE value '${OpenGL_GL_PREFERENCE}' not recognized.  "
            "Allowed values are 'GLVND' only ('LEGACY' is not allowed)."
            )
    endif()
    find_package(OpenGL 4.6 REQUIRED)
else()
    find_package(OpenGL REQUIRED)
endif()

# Status message
if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OPENGL libraries not found - required for GLFW")
else()
    message("Found OPENGL")
    message("      libraries:  ${OPENGL_LIBRARIES}")

    set(GLAD_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/vendors/glad/include")
    set(GLAD_SOURCES "${PROJECT_SOURCE_DIR}/vendors/glad/src/glad")

    # Setup target for glad (glad::glad is reserved)
    add_library(glad SHARED ${GLAD_SOURCES})

    target_include_directories(glad
        PUBLIC
            ${GLAD_INCLUDE_DIRS}
    )
endif()



# ------------------------------------------------------------------------------
# Find - GLFW
# ------------------------------------------------------------------------------
if(TEFNOUT_USE_PRECOMPILED_GLFW)
    set(TEFNOUT_GLFW_ROOT_DIR "${PROJECT_SOURCE_DIR}/vendors/glfw/build/install/${CMAKE_BUILD_TYPE}"
        CACHE STRING "Path used to search first for GLFW header files." FORCE)
    set(GLFW_ROOT_DIR ${TEFNOUT_GLFW_ROOT_DIR})
    find_package(GLFW QUIET)  # QUIET needed to be able to test multiple strategies

    if(NOT GLFW_FOUND)
        message(FATAL_ERROR "GLFW library not found")
    else()
        message(STATUS "Found GLFW ${GLFW_VERSION}")
        message(STATUS "      headers: ${GLFW_INCLUDE_DIRS}")
        message(STATUS "      libraries: ${GLFW_LIBRARIES}")
    endif()
else()
    # Using GLFW project cmake
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory("${PROJECT_SOURCE_DIR}/vendors/glfw")
endif()


# ------------------------------------------------------------------------------
# Create a static or shared library target.
# ------------------------------------------------------------------------------
if (TEFNOUT_BUILD_SHARED_LIBS)
    set (TEFNOUT_LIB_NAME tefnout-shared)
    set (LIB_TYPE SHARED)  # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
else()
    set (TEFNOUT_LIB_NAME tefnout-static)
    set (LIB_TYPE STATIC)  # Set the LIB_TYPE variable to STATIC
endif ()

# Display information about the build parameters
message(STATUS "${TEFNOUT_LIB_NAME} will be build as ${LIB_TYPE} library (${CMAKE_BUILD_TYPE})")


file(
    GLOB
    TEFNOUT_SOURCES
    src/*
    src/Tefnout/Utility/*
    src/Tefnout/Core/*
    src/Tefnout/Window/*
    src/Tefnout/Window/GLFW/*
    src/Tefnout/Rendering/*
    src/Tefnout/Rendering/OpenGL/*
    src/Tefnout/Rendering/Camera/*
)
add_library(${TEFNOUT_LIB_NAME} ${LIB_TYPE} ${TEFNOUT_SOURCES})

# See https://cmake.org/pipermail/cmake/2016-May/063400.html
if (TEFNOUT_BUILD_SHARED_LIBS)
    # Shared library
    target_include_directories(${TEFNOUT_LIB_NAME}
        PRIVATE
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    )
    # Set pre processor command for Shared lib export
    target_compile_definitions(${TEFNOUT_LIB_NAME}
    PRIVATE
        TEFNOUT_SHARED="USED"
        TEFNOUT_SHARED_EXPORTS="USED"
    )
else()
    # Static library
    target_include_directories(${TEFNOUT_LIB_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    )
endif()

# Expose SPDLOG as a PUBLIC dependency
# because it is used in both header and source files
target_link_libraries(${TEFNOUT_LIB_NAME}
    PUBLIC
        spdlog::spdlog
        glm::glm
        glad
        stb_image
    PRIVATE
        glfw
)
set_target_properties(${TEFNOUT_LIB_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)

# Read only alias
add_library(tefnout::tefnout ALIAS ${TEFNOUT_LIB_NAME})




# ------------------------------------------------------------------------------
# Build options based on platform (Windows, Unix)
# ------------------------------------------------------------------------------
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(${TEFNOUT_LIB_NAME}
        PRIVATE
            /W4
            # /WX
    )
    target_link_options(${MY_LIB_NAME}
        PUBLIC
            -MACHINE:x64
    )
else()
    # lots of warnings and all warnings as errors
    target_compile_options(${TEFNOUT_LIB_NAME}
        PRIVATE
            -Wall -Wextra -pedantic -Wwrite-strings
            -Wno-parentheses -Warray-bounds -Weffc++ -Wstrict-aliasing
            # -Werror
        PUBLIC
            -m64
    )
endif()




# ------------------------------------------------------------------------------
# Build options based on build type (Debug, Release, Distribution)
# ------------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(TEFNOUT_LOG_LEVEL 0)
    set(TEFNOUT_DEBUG 1)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(TEFNOUT_LOG_LEVEL 3)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Distribution")
    set(TEFNOUT_LOG_LEVEL 4)
endif()

message(STATUS "${TEFNOUT_LIB_NAME} logging level set to ${TEFNOUT_LOG_LEVEL} ${CMAKE_CPP_FLAGS}")

target_compile_definitions(${TEFNOUT_LIB_NAME}
    PRIVATE
        TEFNOUT_LOG_LEVEL=${TEFNOUT_LOG_LEVEL}
)



# ------------------------------------------------------------------------------
# Add a list of header files to precompile.
# https://cmake.org/cmake/help/latest/command/target_precompile_headers.html
# ------------------------------------------------------------------------------
target_precompile_headers(${TEFNOUT_LIB_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/TefnoutPCH.hpp
)
