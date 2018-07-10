cmake_minimum_required(VERSION 3.9.0)

set(CMAKE_CXX_FLAGS "-g -std=gnu++2a -Werror \
                     -Wno-error=variadic-macros \
                     -Werror=implicit-fallthrough")

set(SOURCES ../src/token.cpp
            ../src/var_substring.cpp
            ../src/cui.cpp
            ../src/message_exception.cpp)

add_executable(lexertl_gen.out main_lexertl_gen.cpp ${SOURCES})
