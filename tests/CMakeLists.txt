cmake_minimum_required(VERSION 3.12.0)

add_subdirectory(/usr/local/include/googletest ../gtets-build) #${GTEST_ROOT} ../gtets-build)

add_executable(${PROJECT_NAME} tests.cpp)
target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock calamity_core)
