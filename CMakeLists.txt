cmake_minimum_required(VERSION 4.0)
project(rlengine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/raylib)
add_subdirectory(external/json)
add_subdirectory(external/spdlog)

include(cmake/imgui.cmake)
include(cmake/rlimgui.cmake)

add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(sandbox)