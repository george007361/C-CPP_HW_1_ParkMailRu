cmake_minimum_required(VERSION 3.20)

project(DigraphsAnalyser)

add_subdirectory(lib/utils)
add_subdirectory(lib/digraphs)
add_subdirectory(lib/parallel_parser)
add_subdirectory(lib/serial_parser)

add_executable(main src/main.c)
# target_link_libraries(main digraphs serial_parser) 
target_link_libraries(main digraphs parallel_parser) 
