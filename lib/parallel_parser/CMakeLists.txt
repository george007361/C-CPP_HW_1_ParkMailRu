add_library(parallel_parser src/parallel_parser.c)

target_include_directories(parallel_parser PUBLIC include)

target_link_libraries(parallel_parser PUBLIC digraphs)
target_link_libraries(parallel_parser PUBLIC utils)