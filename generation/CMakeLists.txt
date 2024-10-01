set(SOURCES
    graph.cpp
    IRBuilder.cpp
    basic_block.cpp
    instruction.cpp
)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/utils)
add_library(generators ${SOURCES})

add_executable(test_factorial factorial_generation.cpp)

target_link_libraries(test_factorial PUBLIC generators)

add_custom_target(
    run_factorial_test
    COMMAND ./test_factorial
)
add_dependencies(run_factorial_test test_factorial)