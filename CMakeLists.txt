cmake_minimum_required(VERSION 2.6)

project(HW1_1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SOURCE_FILES computers.c)
add_library(computers STATIC ${LIB_SOURCE_FILES})

add_executable(main main.c)
target_link_libraries(main computers)


