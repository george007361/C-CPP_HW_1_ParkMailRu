cmake_minimum_required(VERSION 2.6)

project(HW1_1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("--coverage")
add_link_options("--coverage")

set(LIB_SOURCE_FILES menu.c)
add_library(menu STATIC ${LIB_SOURCE_FILES})

set(LIB_SOURCE_FILES computers.c)
add_library(computers STATIC ${LIB_SOURCE_FILES})

add_executable(main main.c)
target_link_libraries(main menu computers)

enable_testing()
find_package(GTest REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests tests.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES} menu computers Threads::Threads )


