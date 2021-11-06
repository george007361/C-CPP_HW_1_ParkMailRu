add_library(graphs SHARED src/graphs.c)

target_include_directories(graphs PUBLIC include)

if(TEST_GRAPHS)
    add_subdirectory(tests)
endif()