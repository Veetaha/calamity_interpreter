cmake_minimum_required(VERSION 3.12.0)

set(INCLUDE_ROOT "include")
set(BOOST_ROOT   "~/libs/boost_1_67_0")
set(LEXERTL_ROOT "~/libs/lexertl")
set(GTEST_ROOT   "/home/tegeran/libs/googletest")

set(CMAKE_CXX_FLAGS "-g -std=gnu++2a -Werror \
                     -Wno-error=variadic-macros \
                     -Werror=implicit-fallthrough")

file(GLOB SOURCES "src/*.cpp")
include_directories(${INCLUDE_ROOT} ${BOOST_ROOT} ${LEXERTL_ROOT})

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

#project(lexer_test.out)
#add_executable(${PROJECT_NAME} tests/lexer_test.cpp ${SOURCES} )
#target_link_libraries(${PROJECT_NAME} m check subunit m rt pthread)
