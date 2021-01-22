cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
project("Tefnout-Samples")



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
# Engine LIB
# ------------------------------------------------------------------------------
add_subdirectory(Tefnout)



# ------------------------------------------------------------------------------
# Samples EXE
# ------------------------------------------------------------------------------
file(
    GLOB
    samples_SOURCES
    ${CMAKE_SOURCE_DIR}/Samples/src/*
)
add_executable(
    tefnout-samples ${samples_SOURCES}
)

# See https://cmake.org/pipermail/cmake/2016-May/063400.html
if (Tefnout_BUILD_SHARED_LIBS)
    # Shared lib need to know includes directories
    message(VERBOSE "Using shared library.")
    target_include_directories(tefnout-samples
        PRIVATE
            ${CMAKE_SOURCE_DIR}/Tefnout/src
            ${GLM_INCLUDE_DIRS}
            ${CMAKE_SOURCE_DIR}/Samples/src
    )

    # Set pre processor command for Shared lib import
    target_compile_definitions(tefnout-samples
    PRIVATE
        TEFNOUT_SHARED="USED"
    )
else()
    message(VERBOSE "Using static library.")
    target_include_directories(tefnout-samples
        PRIVATE
            ${CMAKE_SOURCE_DIR}/Samples/src
    )
endif()


set_target_properties(tefnout-samples
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)
# Using dynamic or static lib based on previous flag (see ${MY_LIB_NAME})
target_link_libraries(tefnout-samples
    PRIVATE
      tefnout
)



# ------------------------------------------------------------------------------
# Only used to debug flags
# ------------------------------------------------------------------------------

# # Informations (Debug)
# message("Your C++ compiler supports these C++ features:")
# foreach(i ${CMAKE_CXX_COMPILE_FEATURES})
#     message("${i}")
# endforeach()

# message("Your CMake version supports these C++ features:")
# get_property(known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
# foreach(i ${known_features})
#     message("${i}")
# endforeach()

# # Get all flags
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
