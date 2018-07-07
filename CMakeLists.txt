cmake_minimum_required(VERSION 2.8.9)

SET(CMAKE_CXX_FLAGS "-g -std=gnu++2a  -Werror -Wno-error=variadic-macros")
# -Wno-error=implicit-fallthrough)
#                   -Wno-error=incompatible-pointer-types")

file(GLOB SOURCES "src/*.cpp")

project (lexertl_gen.out)
include_directories(include)
include_directories(/home/tegeran/libs/boost_1_67_0)
include_directories(/home/tegeran/libs/lexertl)

add_executable(${PROJECT_NAME} main_lexertl_gen.cpp src/cui.cpp src/token.cpp)
target_link_libraries(${PROJECT_NAME} m progbase)

#----------------------

project (calamity.out)
include_directories(include)
include_directories(/home/tegeran/libs/boost_1_67_0)
include_directories(/home/tegeran/libs/lexertl)

add_executable(${PROJECT_NAME} main_calamity.cpp ${SOURCES} src/var_substring.cpp)
target_link_libraries(${PROJECT_NAME} m progbase)



#project (task1.out)
#include_directories(include)
#file(GLOB SOURCES "src/*.cpp")
#add_executable(${PROJECT_NAME} task1.c ${SOURCES})
#target_link_libraries(${PROJECT_NAME} m progbase)



#
##-----------------------
#
#project(task1_test.out)
#add_executable(${PROJECT_NAME} tests/task1_test.c ${SOURCES})
#target_link_libraries(${PROJECT_NAME} progbase check subunit m rt pthread)
#
##------------------------
#project (task2.out)
#include_directories(include)
#file(GLOB SOURCES "src/*.cpp")
#add_executable(${PROJECT_NAME} task2.c ${SOURCES})
#target_link_libraries(${PROJECT_NAME} m progbase)



#------------------

#
#project(task2_test.out)
#add_executable(${PROJECT_NAME} tests/task2_test.c ${SOURCES})
#target_link_libraries(${PROJECT_NAME} m progbase check subunit m rt pthread)
#
##----------------------
#
#project(task3_test.out)
#add_executable(${PROJECT_NAME} tests/task3_test.c ${SOURCES} )
#target_link_libraries(${PROJECT_NAME} m progbase check subunit m rt pthread)
#
##----------------------