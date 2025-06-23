cmake_minimum_required(VERSION 3.10.0)
project(aegis)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

add_executable(
    aegis
    src/Position.cpp
    src/Option.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Frame_manager.cpp
    src/World_map.cpp
    src/Battle.cpp
    src/input.cpp
    src/prompt.cpp
    src/utils.cpp
    src/main.cpp
)

target_include_directories(aegis PRIVATE ${PROJECT_SOURCE_DIR}/src/)