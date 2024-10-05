cmake_minimum_required(VERSION 3.10)
project(kingom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE "GLVND")

find_package(PkgConfig REQUIRED)

find_package(OpenGL REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

file(GLOB_RECURSE SOURCES "src/*.cc")

include_directories(${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(kingom ${SOURCES})

target_include_directories(kingom PRIVATE ${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(kingom ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})