cmake_minimum_required(VERSION 3.9.0)

set(CMAKE_CXX_FLAGS "-g -std=gnu++2a -Werror \
                     -Wno-error=variadic-macros \
                     -Werror=implicit-fallthrough")

add_subdirectory(/usr/local/include/googletest ../gtets-build)

add_executable(${PROJECT_NAME} tests.cpp)
target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock calamity_core)
