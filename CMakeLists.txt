cmake_minimum_required(VERSION 3.20)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled main.cpp main2.cpp)

# Добавление связи с библиотекой comctl32
target_link_libraries(untitled comctl32)