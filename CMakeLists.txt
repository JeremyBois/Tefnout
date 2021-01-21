cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
project("Tefrenout-Samples")

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

    message("*** ${PROJECT_NAME} build as ${CMAKE_BUILD_TYPE}.")
    message("*** C++ compiler is ${CMAKE_CXX_COMPILER} and C compiler is ${CMAKE_C_COMPILER}.")
else()
    # Display information about the build parameters
    message("*** ${PROJECT_NAME} will be build for multi-configuration generator (${CMAKE_CONFIGURATION_TYPES}")
endif()



# ------------------------------------------------------------------------------
# Engine (tefrenout) LIB
# ------------------------------------------------------------------------------
add_subdirectory(Tefrenout)


# ------------------------------------------------------------------------------
# Samples EXE
# ------------------------------------------------------------------------------
file(
    GLOB
    samples_SOURCES
    ${CMAKE_SOURCE_DIR}/Samples/src/*
)
add_executable(
    tefrenout-samples ${samples_SOURCES}
)

# See https://cmake.org/pipermail/cmake/2016-May/063400.html
if (Tefrenout_BUILD_SHARED_LIBS)
    # Shared lib need to know includes directories
    message("*** Using shared library.")
    target_include_directories(tefrenout-samples
        PRIVATE
            ${CMAKE_SOURCE_DIR}/Tefrenout/src
            ${GLM_INCLUDE_DIRS}
            ${CMAKE_SOURCE_DIR}/Samples/src
    )

    # Set pre processor command for Shared lib import
    target_compile_definitions(tefrenout-samples
    PRIVATE
        TEFRENOUT_SHARED="USED"
    )
else()
    message("*** Using static library.")
    target_include_directories(tefrenout-samples
        PRIVATE
            ${CMAKE_SOURCE_DIR}/Samples/src
    )
endif()


set_target_properties(tefrenout-samples
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/${PROJECT_NAME}/"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)
# Using dynamic or static lib based on previous flag (see ${MY_LIB_NAME})
target_link_libraries(tefrenout-samples
    PRIVATE
      tefrenout
)
