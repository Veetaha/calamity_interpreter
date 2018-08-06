cmake_minimum_required(VERSION 3.9.0)

set(SOURCES ../src/token.cpp
            ../src/cui.cpp
            ../src/message_exception.cpp)

add_executable(lexertl_gen.out main_lexertl_gen.cpp ${SOURCES})
