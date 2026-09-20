cmake_minimum_required(VERSION 3.16)
project(wish LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
  message(FATAL_ERROR
    "wish використовує POSIX API (fork/execv/waitpid) і не збирається під MSVC/Windows. "
    "У Visual Studio 2022 оберіть конфігурацію WSL2 (пресет wsl-debug).")
endif()

add_executable(wish
  src/main.cpp
  src/error.cpp
  src/parser.cpp
  src/shell.cpp
)

target_include_directories(wish PRIVATE src)
target_compile_options(wish PRIVATE -Wall -Wextra)

# Тестер OSTEP запускає ./wish з кореня проєкту
set_target_properties(wish PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
