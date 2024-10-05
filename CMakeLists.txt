cmake_minimum_required(VERSION 3.10)
project(kingom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE "GLVND")

if(DEFINED ENV{VCPKG_ROOT})
    message(STATUS "Using vcpkg to find libraries...")
    find_package(glfw3 CONFIG REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glm REQUIRED)
else()
    message(STATUS "Using system libraries (pkg-config)...")
    find_package(PkgConfig REQUIRED)
    find_package(OpenGL REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    pkg_search_module(GLEW REQUIRED glew)
    pkg_search_module(GLM REQUIRED glm)

    if(NOT GLFW_FOUND)
        message(FATAL_ERROR "GLFW3 was not found. Please install the library using your package manager, or use the vcpkg setup. Make sure to provide -DCMAKE_TOOLCHAIN_FILE=\"$($env:VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake\" in your CMake command.")
    endif()
    if(NOT GLEW_FOUND)
        message(FATAL_ERROR "GLEW was not found. Please install the library using your package manager, or use the vcpkg setup. Make sure to provide -DCMAKE_TOOLCHAIN_FILE=\"$($env:VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake\" in your CMake command.")
    endif()
endif()

file(GLOB_RECURSE SOURCES "src/*.cc")

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(kingom ${SOURCES})

if(DEFINED ENV{VCPKG_ROOT})
    target_link_libraries(kingom PRIVATE glfw OpenGL::GL GLEW::GLEW)
else()
    include_directories(${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
    target_link_libraries(kingom PRIVATE ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
endif()
