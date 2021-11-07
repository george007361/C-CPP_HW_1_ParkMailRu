cmake_minimum_required(VERSION 3.12)

project(DigraphsAnalyser)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -fPIC)
set(BUILD_TESTS OFF)
option(TEST_GRAPHS "Enable graphs library test" OFF)
option(TEST_PARSER "Enable parser library test" OFF)
option(TEST_ALL "Enable all tests" OFF)
option(WITH_COVERAGE "Add coverage estimation")
option(WITH_SANITIZERS "Add address sanitizer")

if(WITH_COVERAGE)
    set(BUILD_TESTS ON)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

if(WITH_SANITIZERS)
    add_compile_options(-fno-omit-frame-pointer -g -fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

if(TEST_ALL)
    set(TEST_GRAPHS  ON)
    set(TEST_PARSER  ON)
    set(BUILD_TESTS  ON)
endif()

if(BUILD_TESTS)
    enable_testing()
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
endif()


add_subdirectory(lib/utils)
add_subdirectory(lib/graphs)
option(USE_SERIAL "Enable using parallel library" NO)
if(USE_SERIAL)
    message("Using serial parser")
    add_subdirectory(lib/serial_parser) 
else()
    message("Using parallel parser")
    add_subdirectory(lib/parallel_parser)
endif()


add_executable(main src/main.c)
target_link_libraries(main graphs parser)
