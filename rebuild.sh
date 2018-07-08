#!/usr/bin/env bash
cd lexertl_gen
sh run.sh
cd ..
buildDir="cmake-build-debug"
if [ -d ${buildDir} ] ; then
    rm -rd ${buildDir}
fi
mkdir ${buildDir}
cd    ${buildDir}
cmake ..
make