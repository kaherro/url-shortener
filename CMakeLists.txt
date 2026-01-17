cmake_minimum_required(VERSION 3.16)
project(url-shortener)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(DOWNLOAD 
    "https://raw.githubusercontent.com/chriskohlhoff/asio/asio-1-30-2/asio/include/asio.hpp"
    "${CMAKE_BINARY_DIR}/asio.hpp"
    SHOW_PROGRESS
)

file(DOWNLOAD 
    "https://github.com/mattn/crow-bbs/raw/master/crow_all.h"
    "${CMAKE_BINARY_DIR}/crow_all.h"
    SHOW_PROGRESS
)

include(FetchContent)
FetchContent_Declare(
    sqlite
    URL https://www.sqlite.org/2024/sqlite-amalgamation-3450200.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(sqlite)

add_library(sqlite3_lib STATIC)
target_sources(sqlite3_lib PRIVATE ${sqlite_SOURCE_DIR}/sqlite3.c)
target_include_directories(sqlite3_lib PUBLIC ${sqlite_SOURCE_DIR})
set_target_properties(sqlite3_lib PROPERTIES LINKER_LANGUAGE C)

add_executable(main 
    src/main.cpp
    src/database.cpp
    src/base62.cpp
)

target_include_directories(main PRIVATE 
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
)

target_compile_definitions(main PRIVATE ASIO_STANDALONE)

target_link_libraries(main 
    PRIVATE 
    pthread 
    sqlite3_lib
)