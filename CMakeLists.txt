cmake_minimum_required(VERSION 3.26)
project(CLAP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Release")

set(SRC_DIR src)

include_directories(${SRC_DIR})

add_compile_options("-W")
add_compile_options("-Wall")
add_compile_options("-Wextra")
add_compile_options("-Wpedantic")

include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

add_executable(CLAP
    ${SRC_DIR}/cfg/cli_parser.cpp
    ${SRC_DIR}/cfg/config.cpp
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/lexer/file_reader.cpp
    ${SRC_DIR}/lexer/lexer.cpp
    ${SRC_DIR}/lexer/stat.cpp
    ${SRC_DIR}/lexer/token.cpp
    ${SRC_DIR}/test_main.cpp
)

target_link_libraries(CLAP
    argparse
)
