cmake_minimum_required (VERSION 3.2 FATAL_ERROR)

project (demo)
set (demo_VERSION_MAJOR 0)
set (demo_VERSION_MINOR 0)
set (demo_VERSION_PATCH 1)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

find_package (OpenGL REQUIRED)
find_package (GLEW REQUIRED)
find_package (GLFW REQUIRED)
find_package (GLM REQUIRED)

add_subdirectory (client)
