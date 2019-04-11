#!/bin/bash

if [ -z "$TRAVIS_TAG" ]; then
  export SISTERRAY_VERSION=${TRAVIS_BRANCH}
else
  export SISTERRAY_VERSION=${TRAVIS_TAG:1}
fi

mkdir build
cd build
cmake .. -G "Visual Studio 15 2017" -DVERSION="$SISTERRAY_VERSION"
cmake --build . --config Release
cmake --build . --config Release --target package
