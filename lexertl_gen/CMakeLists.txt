cmake_minimum_required(VERSION 3.9.0)

set(PATH_TO_BOOST   "~/libs/boost_1_67_0")
set(PATH_TO_LEXERTL "~/libs/lexertl")
set(MAIN_INCLUDE    "../include")
set(SOURCES ../src/token.cpp
#            ../src/var_string.cpp
#            ../src/var.cpp
            ../src/var_substring.cpp
            ../src/cui.cpp
            ../src/message_exception.cpp)
set(CMAKE_CXX_FLAGS "-g -std=gnu++2a -Werror \
                     -Wno-error=variadic-macros \
                     -Werror=implicit-fallthrough")

include_directories(${MAIN_INCLUDE} ${PATH_TO_LEXERTL} ${PATH_TO_BOOST})

#=========================
project (lexertl_gen.out)
add_executable(lexertl_gen.out main_lexertl_gen.cpp ${SOURCES})
#=========================