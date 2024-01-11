include(GoogleTest)

function(tape_sorter_test_target target)
    add_executable(
            ${target}
            ${target}.cpp
    )
    message(STATUS "Add test ${target}")
    target_link_libraries(${target} PRIVATE ${LIBRARY_NAME})
    target_link_libraries(${target} PRIVATE GTest::gtest_main)
    gtest_discover_tests(${target})
endfunction()
