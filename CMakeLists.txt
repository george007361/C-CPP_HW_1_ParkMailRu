cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME HW_1_ParkMailRu)
project(${PROJECT_NAME})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SOURCE_FILES menu.c)
add_library(menu STATIC ${LIB_SOURCE_FILES})

set(LIB_SOURCE_FILES computers.c)
add_library(computers STATIC ${LIB_SOURCE_FILES})

add_executable(main main.c)
target_link_libraries(main computers menu)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(gtesting gtesting.cpp)
target_link_libraries(gtesting gtest menu computers -lpthread)

