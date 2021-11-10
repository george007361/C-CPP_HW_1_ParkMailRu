add_executable(test_graphs ${PROJECT_SOURCE_DIR}/lib/graphs/tests/src/test_graphs.cpp)
target_link_libraries(test_graphs gtest graphs -lpthread)
add_test(NAME test_graphs COMMAND test_graphs)
