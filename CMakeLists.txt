cmake_minimum_required(VERSION 3.16)
project(ogl_shadow)

set(CMAKE_CXX_STANDARD 14)

# OpenGL
find_package(OpenGL REQUIRED)

add_executable(ogl_shadow glad.c main.cpp shader.h shader.cpp mesh/cube.h mesh/cube.cpp light.h texture/texture.h texture/texture.cpp mesh/model.h mesh/model.cpp camera.cpp camera.h depth_map.h depth_map.cpp mesh/quad.h mesh/quad.cpp)

include_directories(${OPENGL_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(ogl_shadow ${OPENGL_LIBRARIES} ${CMAKE_SOURCE_DIR}/lib/libglfw3.a)