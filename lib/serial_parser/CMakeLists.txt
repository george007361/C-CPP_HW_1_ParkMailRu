add_library(serial_parser src/serial_parser.c)

target_include_directories(serial_parser PUBLIC include)

target_link_libraries(serial_parser PUBLIC digraphs)
target_link_libraries(serial_parser PUBLIC utils)