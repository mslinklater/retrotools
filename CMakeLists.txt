project (retrotool)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_C_FLAGS "-g -DDEBUG")

set(SOURCE_FILES main.c config.c errorcodes.c memory.c)

add_executable(retrotool ${SOURCE_FILES})

