cmake_minimum_required(VERSION 3.16)
project(ogl_shadow)

set(CMAKE_CXX_STANDARD 14)

# OpenGL
find_package(OpenGL REQUIRED)

#Search for GLFW library
find_library(GLFW3_LIBRARY glfw3 "${CMAKE_SOURCE_DIR}/lib")

add_executable(ogl_shadow glad.c main.cpp shader.h shader.cpp cube.h cube.cpp)

include_directories(${OPENGL_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(ogl_shadow ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY})