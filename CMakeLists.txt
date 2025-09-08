cmake_minimum_required(VERSION 3.10)
project(Hydrogen)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(hel src/main.cpp
        src/tokenizer.hpp
        src/parser.hpp
        src/utils.hpp
        src/generator.hpp)