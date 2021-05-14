cmake_minimum_required(VERSION 3.3.0 FATAL_ERROR)
project("Tefnout-samples")

# ------------------------------------------------------------------------------
# Engine SAMPLES
# ------------------------------------------------------------------------------
file(
    GLOB
    samples_SOURCES
    ${PROJECT_SOURCE_DIR}/src/*
)
add_executable(
    tefnout-samples ${samples_SOURCES}
)

# See https://cmake.org/pipermail/cmake/2016-May/063400.html
if (TEFNOUT_BUILD_SHARED_LIBS)
    # Shared lib need to know includes directories
    message(VERBOSE "Using shared library.")
    target_include_directories(tefnout-samples
        PRIVATE
            ${CMAKE_SOURCE_DIR}/Tefnout/src
            ${PROJECT_SOURCE_DIR}/src
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
            ${CMAKE_SOURCE_DIR}/Tefnout/src
            ${PROJECT_SOURCE_DIR}/src
    )
endif()


set_target_properties(tefnout-samples
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${TEFNOUT_LIB_TYPE}/${CMAKE_GENERATOR_PLATFORM}/Samples/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${TEFNOUT_LIB_TYPE}/${CMAKE_GENERATOR_PLATFORM}/Samples/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${TEFNOUT_LIB_TYPE}/${CMAKE_GENERATOR_PLATFORM}/Samples/"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)
# Using dynamic or static lib
target_link_libraries(tefnout-samples
    PRIVATE
      tefnout::tefnout
)
