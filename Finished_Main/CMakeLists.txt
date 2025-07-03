cmake_minimum_required(VERSION 3.31)
project(Assignment_5)

set(CMAKE_CXX_STANDARD 20)
add_library(sqlite3 libraries/C++/SQLite/sqlite3.c)
add_executable(Assignment_5 main.cpp
        ADMIN.cpp
        ADMIN.h
        STUDENT.cpp
        STUDENT.h
        INSTRUCTOR.cpp
        INSTRUCTOR.h
        USER.cpp
        USER.h
        assignment3.db
)
target_link_libraries(Assignment_5 sqlite3)


target_include_directories(Assignment_5 PRIVATE libraries/C++/SQLite)





