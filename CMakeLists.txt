cmake_minimum_required(VERSION 3.16)
project(tape_sorter
        DESCRIPTION "Tape Sorter"
        LANGUAGES CXX
        VERSION 1.0.0)

string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)

set(LIBRARY_NAME ${PROJECT_NAME_LOWERCASE})
set(LIBRARY_FOLDER ${PROJECT_NAME_LOWERCASE})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(${PROJECT_NAME_UPPERCASE}_BUILD_TESTS "Build ${LIBRARY_NAME} tests" OFF)
option(BUILD_SHARED_LIBS "Build ${LIBRARY_NAME} as a shared library." ON)

include(${PROJECT_SOURCE_DIR}/cmake/LibraryBuild.cmake)

add_subdirectory(
        ${PROJECT_SOURCE_DIR}/demos
)

if (${PROJECT_NAME_UPPERCASE}_BUILD_TESTS)
    add_subdirectory(
            ${PROJECT_SOURCE_DIR}/tests
    )
endif ()
