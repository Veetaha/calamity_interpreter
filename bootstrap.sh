#!/usr/bin/env bash
INCLUDE_ROOT="/usr/local/include"

BOOST_TAG="boost_1_67_0"
BOOST_VERSION="1.67.0"
BOOST_SOURCE_DOWNLOAD_LINK="https://sourceforge.net/projects/boost/files/boost/${BOOST_VERSION}/${BOOST_TAG}.tar.gz/download"

LEXERTL_VITAHA_REPO="https://tegeran@bitbucket.org/tegeran/ben_hansons_lexertl14_changed.git"

GOOGLE_TEST_REPO="https://github.com/google/googletest.git"

cd "$INCLUDE_ROOT"

##### install boost
wget -O "$BOOST_TAG".tar.gz "$BOOST_SOURCE_DOWNLOAD_LINK"
tar xzvf "$BOOST_TAG".tar.gz
# should've run "$BOOST_TAG"/bootstrap.sh, but it takes more than half
# an hour and is not necessary as we use header only components

##### install lexertl
git clone "$LEXERTL_VITAHA_REPO"
mv ben_hansons_lexertl14_changed lexertl

##### install googletest
git clone "$GOOGLE_TEST_REPO"






