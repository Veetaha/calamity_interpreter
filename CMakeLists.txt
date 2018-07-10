cmake_minimum_required(VERSION 3.9.0)

set(INCLUDE_ROOT "/usr/local/include")
set(BOOST_ROOT   "${INCLUDE_ROOT}/boost_1_67_0")
set(LEXERTL_ROOT "${INCLUDE_ROOT}/lexertl")
set(GTEST_ROOT   "${INCLUDE_ROOT}/googletest")

set(CMAKE_CXX_FLAGS "-g -std=gnu++2a -Werror \
                     -Wno-error=variadic-macros \
                     -Werror=implicit-fallthrough")

file(GLOB SOURCES "src/*.cpp")
# global include for all projects (use target_link_libraries otherwise)
include_directories(include ${BOOST_ROOT} ${LEXERTL_ROOT} ${GTEST_ROOT})

add_library(calamity_core ${SOURCES})

#==========================================================#
    project (lexer.out)                                    #
    add_executable(${PROJECT_NAME} main_lexer.cpp)         #
    target_link_libraries(${PROJECT_NAME} m calamity_core) #
#==========================================================#

#==========================================================#
    project (parser.out)                                   #
    add_executable(${PROJECT_NAME} main_parser.cpp)        #
    target_link_libraries(${PROJECT_NAME} m calamity_core) #
#==========================================================#

#===========================#
    project(tests)          #
    add_subdirectory(tests) #
#===========================#

#=================================#
    project(lexertl_gen)          #
    add_subdirectory(lexertl_gen) #
#=================================#

# Pure C test framwork linkings:
#target_link_libraries(${PROJECT_NAME} m check subunit m rt pthread)
