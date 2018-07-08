cmake_minimum_required(VERSION 3.12.0)

set(INCLUDE_ROOT "../${INCLUDE_ROOT}")

add_subdirectory(${GTEST_ROOT} ../gtets-build)

include_directories(
        ${BOOST_ROOT}
        ${LEXERTL_ROOT}
        ${INCLUDE_ROOT}
        ${GTEST_ROOT}/googletest
        ${GTEST_ROOT}/googlemock
)
add_executable(${PROJECT_NAME} tests.cpp)
target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock calamity_core)
