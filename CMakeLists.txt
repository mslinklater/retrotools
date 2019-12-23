cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "-DDEBUG")
set(CMAKE_BUILD_TYPE Debug)

if (UNIX)
set(CMAKE_LIBRARY_PREFIXES lib)
set(CMAKE_LIBRARY_SUFFIXES a)
set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES a)

# libraries
set(SDL2_DIR /usr/lib/x86_64-linux-gnu/cmake/SDL2/)
set(OPENGL_opengl_LIBRARY /usr/lib/x86_64-linux-gnu/libOpenGL.so)
set(OPENGL_glx_LIBRARY /usr/lib/x86_64-linux-gnu/)

# headers
set(OPENGL_INCLUDE_DIR /usr/include/GL/)

find_package(SDL2 REQUIRED)
find_package(OpenGL)

include_directories(${SDL2_INCLUDE_DIRS})

endif (UNIX)

if (WIN32)
set(CMAKE_MODULE_PATH C:\\Users\\mlinklater\\Code\\SDL2-2.0.10\\include)
endif (WIN32)


set(SOURCE_FILES commands.cpp config.cpp disasm.cpp errorcodes.cpp main.cpp log.cpp symbol.cpp symbolstore.cpp)

add_subdirectory("imgui")
add_subdirectory("components")
add_subdirectory("system")
add_subdirectory("windows")

project (vistella VERSION 0.1)
configure_file(version.h.in version.h)

add_executable(vistella ${SOURCE_FILES})
target_include_directories(vistella PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(vistella ${SDL2_LIBRARIES} ${OPENGL_opengl_LIBRARY})
target_compile_options(vistella PRIVATE -Wall -Werror)
