cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "-DDEBUG")
set(CMAKE_BUILD_TYPE Debug)

enable_testing()
add_test(cpu6502 build/vistella [cpu6502])

if (UNIX)
set(CMAKE_LIBRARY_PREFIXES lib)
set(CMAKE_LIBRARY_SUFFIXES a)
set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES a)

# libraries
# SDL2
if(EXISTS /usr/lib/arm-linux-gnueabihf/cmake/SDL2/)
	set(SDL2_DIR /usr/lib/arm-linux-gnueabihf/cmake/SDL2/)
endif()

if(EXISTS /usr/lib/arm-linux-gnueabihf/libOpenGL.so)
	set(OPENGL_opengl_LIBRARY /usr/lib/arm-linux-gnueabihf/libOpenGL.so)
endif()

if(EXISTS /usr/lib/arm-linux-gnueabihf/)
	set(OPENGL_glx_LIBRARY /usr/lib/arm-linux-gnueabihf/)
endif()

# headers
set(OPENGL_INCLUDE_DIR /usr/include/GL/)

find_package(SDL2 REQUIRED)
find_package(OpenGL)

include_directories(${SDL2_INCLUDE_DIRS})

endif (UNIX)

if (WIN32)
set(CMAKE_MODULE_PATH C:\\Users\\mlinklater\\Code\\SDL2-2.0.10\\include)
endif (WIN32)


set(SOURCE_FILES commands.cpp config.cpp disasm.cpp main.cpp symbol.cpp symbolstore.cpp sessionmanager.cpp)

add_subdirectory("imgui")
add_subdirectory("components")
add_subdirectory("utils")
add_subdirectory("windows")
add_subdirectory("test")
add_subdirectory("shared_cpp")
add_subdirectory("filebrowser")
add_subdirectory("perfect6502")

project (vistella VERSION 0.1)
configure_file(version.h.in version.h)

add_executable(vistella ${SOURCE_FILES})
target_include_directories(vistella PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(vistella ${SDL2_LIBRARIES} ${OPENGL_opengl_LIBRARY})
target_compile_options(vistella PRIVATE -Wall -Werror -Wno-psabi -g)
