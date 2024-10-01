set(SOURCES
    graph.cpp
    IRGenerator.cpp
    basic_block.cpp
    instruction.cpp
)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/utils)
add_library(generators ${SOURCES})

add_executable(factorial_test tests/factorial_generation.cpp)

target_link_libraries(factorial_test PUBLIC generators)

add_custom_target(
    run_factorial_test
    COMMAND ./factorial_test
)
add_dependencies(run_factorial_test factorial_test)