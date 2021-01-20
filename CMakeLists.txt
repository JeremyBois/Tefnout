cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
project("tefranout")



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

    # x64 as default platform
    # if(NOT CMAKE_GENERATOR_PLATFORM )
    #     message("*** Setting Target platform architecture because none was specified.")
    #     set(CMAKE_GENERATOR_PLATFORM x64 CACHE STRING "Choose the target platform architecture." FORCE)
    #     set_property(CACHE CMAKE_GENERATOR_PLATFORM PROPERTY STRINGS "x86" "x64")
    # endif()
    message("*** ${PROJECT_NAME} build as ${CMAKE_BUILD_TYPE} for architecture ${CMAKE_GENERATOR_PLATFORM}.")
    message("*** C++ compiler is ${CMAKE_CXX_COMPILER} and C compiler is ${CMAKE_C_COMPILER}.")
else()
    # Display information about the build parameters
    message("*** ${PROJECT_NAME} will be build for multi-configuration generator (${CMAKE_CONFIGURATION_TYPES}")
endif()



###
# Engine (tefranout) LIB
###
add_subdirectory(Tefranout)


###
# Samples EXE
###

file(
    GLOB
    samples_SOURCES
    ${CMAKE_SOURCE_DIR}/Samples/src/*
)

add_executable(
    tefranout-samples ${samples_SOURCES}
)

# See https://cmake.org/pipermail/cmake/2016-May/063400.html
if (BUILD_SHARED_LIBS)
    # Shared lib need to know includes directories
    message("*** Using shared library.")
    target_include_directories(tefranout-samples
        PRIVATE
            ${CMAKE_SOURCE_DIR}/Tefranout/src
            ${CMAKE_SOURCE_DIR}/Tefranout/vendors/glm/glm
            ${CMAKE_SOURCE_DIR}/Samples/src
    )

    # Set pre processor command for Shared lib import
    target_compile_definitions(tefranout-samples
    PRIVATE
        TEFRANOUT_SHARED="USED"
    )
else()
    target_include_directories(tefranout-samples
        PRIVATE
            ${CMAKE_SOURCE_DIR}/Samples/src
    )
endif()


set_target_properties(tefranout-samples
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)

# Using dynamic or static lib based on previous flag (see ${MY_LIB_NAME})
target_link_libraries(tefranout-samples
    PRIVATE tefranout
)
