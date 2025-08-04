cmake_minimum_required(VERSION 3.31)
project(Assignment_5)

set(CMAKE_CXX_STANDARD 20)


# Include FetchContent for Google Test
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# Download and make available
FetchContent_MakeAvailable(googletest)

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

# Enable testing
enable_testing()

# Add your test executable
add_executable(unit_tests
        tests/test_main.cpp
        tests/test_ADMIN.cpp
        tests/test_INSTRUCTOR.cpp
)

# Link test target with gtest and your project files
target_link_libraries(unit_tests
        gtest
        gtest_main
        sqlite3
)

target_sources(unit_tests PRIVATE
        ADMIN.cpp STUDENT.cpp INSTRUCTOR.cpp USER.cpp
)

# Optionally include your source files if you're testing them
target_sources(unit_tests PRIVATE
        ADMIN.cpp
        STUDENT.cpp
        INSTRUCTOR.cpp
        USER.cpp
)
target_include_directories(unit_tests PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/libraries/C++/SQLite
)

# Add GTest to CTest
include(GoogleTest)
gtest_discover_tests(unit_tests)

