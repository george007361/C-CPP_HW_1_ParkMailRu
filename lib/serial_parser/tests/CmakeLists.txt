add_executable(test_parser src/test_serial_parser.cpp)
target_link_libraries(test_parser gtest parser -lpthread)
add_test(NAME test_parser COMMAND test_parser)
