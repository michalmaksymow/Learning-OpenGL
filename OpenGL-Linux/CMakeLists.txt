cmake_minimum_required(VERSION 3.15)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable(application application.cpp renderer.cpp renderer.hpp)

add_subdirectory(include/glfw-3.3.2)
target_link_libraries(application glfw)

add_subdirectory(include/glew-2.1.0/build/cmake)
target_link_libraries(application glew)

find_package(OpenGL REQUIRED)
target_link_libraries(application OpenGL::GL)
