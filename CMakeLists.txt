cmake_minimum_required(VERSION 3.10)

project (distella)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_C_FLAGS "-g -DDEBUG")

find_library(NCURSES_LIB ncurses)

message(STATUS ${NCURSES_LIB})

set(SOURCE_FILES main.c disasm.c config.c errorcodes.c memory.c cpu.c vcs.c)

add_executable(distella ${SOURCE_FILES})
target_link_libraries(distella "${NCURSES_LIB}")

