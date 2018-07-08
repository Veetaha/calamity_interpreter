#!/usr/bin/env bash
buildDir=cmake-build-debug
if [ ! -d "$buildDir" ] ; then
    echo "No \"${buildDir}\" folder was found, rebuilding lexertl_gen project from scratch"
    sh rebuild.sh
else
    cd $buildDir
    make
fi

