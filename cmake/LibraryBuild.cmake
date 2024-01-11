set(LIBRARY_HEADER_FILES
        ${PROJECT_SOURCE_DIR}/include/tape_sorter/tape_interface.h
        ${PROJECT_SOURCE_DIR}/include/tape_sorter/file_tape.h
        ${PROJECT_SOURCE_DIR}/include/tape_sorter/sort/tape_sorter.h
        ${PROJECT_SOURCE_DIR}/include/tape_sorter/sort/temp_tape_creator.h
        ${PROJECT_SOURCE_DIR}/include/tape_sorter/sort/temp_file_tape_creator.h
        ${PROJECT_SOURCE_DIR}/include/tape_sorter/delay_config/tape_delay_config_parser.h
        ${PROJECT_SOURCE_DIR}/include/tape_sorter/delay_config/tape_delay_config.h
)

set(LIBRARY_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/tape_sorter/file_tape.cpp
        ${PROJECT_SOURCE_DIR}/src/tape_sorter/sort/tapes_priority_queue.h
        ${PROJECT_SOURCE_DIR}/src/tape_sorter/sort/tape_sorter.cpp
        ${PROJECT_SOURCE_DIR}/src/tape_sorter/delay_config/tape_delay_config_parser.cpp
        ${PROJECT_SOURCE_DIR}/src/tape_sorter/sort/temp_file_tape_creator.cpp
)

add_library(${LIBRARY_NAME}
        ${LIBRARY_SOURCE_FILES}
        ${LIBRARY_HEADER_FILES}
)

add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

target_include_directories(
        ${LIBRARY_NAME} PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

include(FetchContent)

if (${PROJECT_NAME_UPPERCASE}_BUILD_TESTS)
    message(STATUS "FetchContent: googletest")
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
endif ()
