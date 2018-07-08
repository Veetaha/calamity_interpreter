#!/usr/bin/env bash
executable="./cmake-build-debug/lexertl_gen.out"
target="../include/lexertl_gen.h"
sh build.sh
if [ -f "$executable" ] ; then
    ${executable} ${target}
else
    echo "Build process failed, nothing to run"
fi

