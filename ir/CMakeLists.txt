set(SOURCES
    graph.cpp
    IRGenerator.cpp
    basic_block.cpp
    instruction.cpp
)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/utils)
add_library(generators ${SOURCES})

add_custom_target(run_all_tests)

add_subdirectory(tests)