cmake_minimum_required(VERSION 3.10)

project (distella)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_C_FLAGS "-DDEBUG")
set(CMAKE_BUILD_TYPE Debug)

find_library(NCURSES_LIB ncurses)

message(STATUS ${NCURSES_LIB})

set(SOURCE_FILES main.cpp disasm.cpp config.cpp errorcodes.cpp memory.cpp cpu.cpp vcs.cpp)

add_executable(distella ${SOURCE_FILES})
target_link_libraries(distella "${NCURSES_LIB}")

