cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_FLAGS "-DDEBUG")
set(CMAKE_BUILD_TYPE Debug)

if (UNIX)
set(CMAKE_LIBRARY_PREFIXES lib)
set(CMAKE_LIBRARY_SUFFIXES a)
set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES a)
endif (UNIX)

if (WIN32)
set(CMAKE_MODULE_PATH C:\\Users\\mlinklater\\Code\\SDL2-2.0.10\\include)
endif (WIN32)

set(SOURCE_FILES main.cpp disasm.cpp config.cpp errorcodes.cpp log.cpp symbolstore.cpp symbol.cpp commands.cpp machine/vcs.cpp)

set(SYSTEM_FILES system/command.cpp system/memoryutils.cpp system/windowbase.cpp system/windowmanager.cpp)

set(COMPONENT_FILES components/cpu6502.cpp components/memory.cpp components/tia.cpp)

set(IMGUI_FILES imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_impl_sdl.cpp imgui/imgui_impl_opengl2.cpp imgui/imgui_widgets.cpp)

set(WINDOW_FILES windows/logwindow.cpp windows/memorywindow.cpp windows/disasmwindow.cpp windows/mainwindow.cpp windows/symbolwindow.cpp windows/cpu6502window.cpp windows/tiawindow.cpp)

# libraries
set(SDL2_DIR /usr/lib/x86_64-linux-gnu/cmake/SDL2/)
set(OPENGL_opengl_LIBRARY /usr/lib/x86_64-linux-gnu/libOpenGL.so)
set(OPENGL_glx_LIBRARY /usr/lib/x86_64-linux-gnu/)

# headers
set(OPENGL_INCLUDE_DIR /usr/include/GL/)

find_package(SDL2 REQUIRED)
find_package(OpenGL)

include_directories(${SDL2_INCLUDE_DIRS})

project (vistella)
add_executable(vistella ${SOURCE_FILES} ${IMGUI_FILES} ${WINDOW_FILES} ${COMPONENT_FILES} ${SYSTEM_FILES})
target_link_libraries(vistella ${SDL2_LIBRARIES} ${OPENGL_opengl_LIBRARY})
target_compile_options(vistella PRIVATE -Wall -Werror)
