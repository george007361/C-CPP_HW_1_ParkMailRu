cmake_minimum_required(VERSION 3.20)

project(DigraphsAnalyser)

add_compile_options(-Wall -Wextra -pedantic)

option(WITH_COVERAGE "Add coverage estimation")
if(WITH_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

option(WITH_SANITIZERS "Add address sanitizer")
if(WITH_SANITIZERS)
    add_compile_options(-fno-omit-frame-pointer -fsanitize=address)
    add_link_options(-fsanitize=address)
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