cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
project("Tefnout-Tests")


# ------------------------------------------------------------------------------
# Engine TESTS
# ------------------------------------------------------------------------------
add_subdirectory(${PROJECT_SOURCE_DIR}/vendors/Catch2)

list(APPEND tests_SOURCES
    ${PROJECT_SOURCE_DIR}/src/CatchMain.cpp
    ${PROJECT_SOURCE_DIR}/src/Container/RingBuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/ECS/FamilyGenerator.cpp
)

add_executable(tefnout-testing ${tests_SOURCES})

set_target_properties(tefnout-testing
PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/Tests/"
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/Tests/"
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_GENERATOR_PLATFORM}/Tests/"
CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
LINKER_LANGUAGE CXX
)

if (TEFNOUT_BUILD_SHARED_LIBS)
    target_include_directories(tefnout-testing
        PRIVATE
            ${CMAKE_SOURCE_DIR}/Tefnout/src
            ${PROJECT_SOURCE_DIR}/src
    )
endif()

target_link_libraries(tefnout-testing
    PRIVATE
        tefnout::tefnout
        Catch2::Catch2
)