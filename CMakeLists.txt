cmake_minimum_required(VERSION 3.20)
project(fex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/arena.cpp
    src/fex.cpp
    src/interpreter.cpp
    src/main.cpp
    src/parser.cpp
    src/scanner.cpp
    src/token.cpp
    src/utils.cpp
)

add_executable(fex ${SOURCES})

target_precompile_headers(fex PRIVATE "src/pch.h")
