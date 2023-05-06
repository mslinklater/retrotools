cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "-DDEBUG -DLUA_USE_POSIX")
set(CMAKE_CXX_FLAGS "-DDEBUG")
set(CMAKE_BUILD_TYPE Debug)

#enable_testing()
#add_test(cpu6502 build/vistella [cpu6502])

if (UNIX)
set(CMAKE_LIBRARY_PREFIXES lib)
set(CMAKE_LIBRARY_SUFFIXES a)
set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES a)

#set(SDL2_DIR /usr/lib/x86_64-linux-gnu/cmake/SDL2/)
#set(OPENGL_opengl_LIBRARY /usr/lib/x86_64-linux-gnu/libOpenGL.so)
#set(OPENGL_glx_LIBRARY /usr/lib/x86_64-linux-gnu/)

# libraries - I'm probably doing this a rubbish way
# SDL2
if(EXISTS /usr/lib/arm-linux-gnueabihf/cmake/SDL2/)
	set(SDL2_DIR /usr/lib/arm-linux-gnueabihf/cmake/SDL2/)
elseif(EXISTS /usr/lib/x86_64-linux-gnu/cmake/SDL2/)
	set(SDL2_DIR /usr/lib/x86_64-linux-gnu/cmake/SDL2/)
endif()

if(EXISTS /usr/lib/arm-linux-gnueabihf/libOpenGL.so)
	set(OPENGL_opengl_LIBRARY /usr/lib/arm-linux-gnueabihf/libOpenGL.so)
elseif(EXISTS /usr/lib/x86_64-linux-gnu/libOpenGL.so)
	set(OPENGL_opengl_LIBRARY /usr/lib/x86_64-linux-gnu/libOpenGL.so)
endif()

if(EXISTS /usr/lib/arm-linux-gnueabihf/)
	set(OPENGL_glx_LIBRARY /usr/lib/arm-linux-gnueabihf/)
elseif(EXISTS /usr/lib/x86_64-linux-gnu/)
	set(OPENGL_glx_LIBRARY /usr/lib/x86_64-linux-gnu/)
endif()

# headers
set(OPENGL_INCLUDE_DIR /usr/include/GL/)

find_package(SDL2 REQUIRED)
find_package(OpenGL)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories("3rdparty")
include_directories(".")

endif (UNIX)

set(SOURCE_FILES main.cpp)

add_subdirectory("3rdparty/imgui")
add_subdirectory("3rdparty/lua-5.4.4/src")
add_subdirectory("components")
#add_subdirectory("components/cpu6502")
#add_subdirectory("components/stella")
add_subdirectory("utils")
add_subdirectory("windows")
add_subdirectory("test")
add_subdirectory("system")
add_subdirectory("resources")
add_subdirectory("machines")
add_subdirectory("3rdparty/perfect6502")

project (retrotools VERSION 0.1 HOMEPAGE_URL https://github.com/mslinklater)

configure_file(version.h.in version.h)

add_executable(retrotools ${SOURCE_FILES})
target_include_directories(retrotools PUBLIC "${PROJECT_BINARY_DIR}" ".")
target_link_libraries(retrotools ${SDL2_LIBRARIES} ${OPENGL_opengl_LIBRARY})
target_compile_options(retrotools PRIVATE -Wall -Werror -Wno-psabi -g)
