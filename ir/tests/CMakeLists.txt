add_executable(factorial_test factorial_generation_test.cpp)
target_link_libraries(factorial_test PUBLIC generators)
add_custom_target(
    run_factorial_test
    COMMAND ./factorial_test
)
add_dependencies(run_factorial_test factorial_test)
add_dependencies(run_all_tests run_factorial_test)

add_executable(graph_tests dtree_tests.cpp loop_analyzer_tests.cpp)
target_link_libraries(graph_tests generators GTest::gtest_main)
target_include_directories(graph_tests PUBLIC
    ${PROJECT_THIRD_PARTY}/googletest
)

add_custom_target(
    run_graph_tests
    COMMENT "Running CFG tests"
    COMMAND ./graph_tests
)
add_dependencies(run_graph_tests graph_tests)
add_dependencies(run_all_tests run_graph_tests)