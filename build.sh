#!/usr/bin/env bash
buildDir="cmake-build-debug"
if [ ! -d "$buildDir" ] ; then
    echo "No ${buildDir} was found, rebuilding calamity_interpreter from scratch"
    sh rebuild.sh
else
    cd lexertl_gen
    sh run.sh
    cd ../${buildDir}
    make
fi