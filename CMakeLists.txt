cmake_minimum_required(VERSION 3.10)
project(HLM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
set(SOURCES
    src/main.cpp
)

# Executable target
add_executable(hel ${SOURCES})

# Include headers from src/
target_include_directories(hel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Enable compiler warnings
if(MSVC)
    target_compile_options(hel PRIVATE /W4)
else()
    target_compile_options(hel PRIVATE -Wall -Wextra -Wpedantic)
endif()