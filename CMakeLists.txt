cmake_minimum_required(VERSION 3.10)

project (distella)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_C_FLAGS "-DDEBUG")
set(CMAKE_BUILD_TYPE Debug)
set(OpenGL_GL_PREFERENCE LEGACY)

set(SOURCE_FILES main.cpp disasm.cpp config.cpp errorcodes.cpp memory.cpp cpu.cpp vcs.cpp log.cpp symbolstore.cpp)

set(IMGUI_FILES imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_impl_sdl.cpp imgui/imgui_impl_opengl2.cpp imgui/imgui_widgets.cpp)

set(LOGWINDOW_FILES windows/logwindow.cpp windows/memorywindow.cpp)

find_package(SDL2 REQUIRED)
find_package(OpenGL)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(distella ${SOURCE_FILES} ${IMGUI_FILES} ${LOGWINDOW_FILES})
target_link_libraries(distella ${SDL2_LIBRARIES} ${OPENGL_gl_LIBRARY})
