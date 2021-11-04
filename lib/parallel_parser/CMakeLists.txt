add_library(parser src/parallel_parser.c)

target_include_directories(parser PUBLIC include)

target_link_libraries(parser PUBLIC graphs)
target_link_libraries(parser PUBLIC utils)