cmake_minimum_required(VERSION 3.30)
project(Assignment_3)

set(CMAKE_CXX_STANDARD 20)
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(Assignment_3 main.cpp)

target_link_libraries(Assignment_3 sqlite3)