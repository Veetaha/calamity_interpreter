#!/usr/bin/env bash
buildDir=cmake-build-debug
if [ -d "$buildDir" ] ; then
    rm -rd "$buildDir"
fi
mkdir "$buildDir"
cd    "$buildDir"
cmake ..
make
