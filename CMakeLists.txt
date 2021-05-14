cmake_minimum_required(VERSION 3.3.0 FATAL_ERROR)
project("Tefnout-main")



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
# Enable this option to also build engine tests
OPTION (TEFNOUT_BUILD_TESTING "Build test suite for Tefnout Engine" OFF)

# ------------------------------------------------------------------------------
# Engine LIB
# ------------------------------------------------------------------------------
add_subdirectory(Tefnout)



# ------------------------------------------------------------------------------
# Engine TESTS
# ------------------------------------------------------------------------------
if(TEFNOUT_BUILD_TESTING)
    add_subdirectory(Tests)
endif()


# ------------------------------------------------------------------------------
# Samples EXE
# ------------------------------------------------------------------------------
add_subdirectory(Samples)



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
