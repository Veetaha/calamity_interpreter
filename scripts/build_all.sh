#!/usr/bin/env bash
set -e
BILD_DIR="cmake-build-debug"
cd ..
if [ ! -d "${BILD_DIR}" ]
then
    echo "No ${BILD_DIR} was found, rebuilding calamity_lexer from scratch"
    mkdir "$BILD_DIR"
    cd "$BILD_DIR"
    cmake -DCMAKE_C_COMPILER=/usr/bin/cc -DCMAKE_CXX_COMPILER=/usr/bin/c++ ..
    cd ..
fi
    cd "$BILD_DIR"/lexertl_gen
    make
#    wait $!
    ./lexertl_gen.out ../../include/lexertl_gen.h
    cd ..
    make
