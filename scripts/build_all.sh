#!/usr/bin/env bash
set -ev
buildDir="cmake-build-debug"
cd ..
if [ ! -d "${buildDir}" ]
then
    echo "No ${buildDir} was found, rebuilding calamity_lexer from scratch"
    mkdir "$buildDir"
    cd "$buildDir"
    cmake ..
    cd ..
fi
    cd "$buildDir"/lexertl_gen
    make
#    wait $!
    ./lexertl_gen.out ../../include/lexertl_gen2.h
    cd ..
    make
