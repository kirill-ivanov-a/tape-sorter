enable_testing()

include(${PROJECT_SOURCE_DIR}/cmake/TestTarget.cmake)
set(TEST_DATA_DIR ${PROJECT_SOURCE_DIR}/tests/data)

tape_sorter_test_target(test_file_tape)
tape_sorter_test_target(test_tape_sort)
tape_sorter_test_target(test_delay_config_parser)
