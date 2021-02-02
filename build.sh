#!/bin/bash

BUILD_DIR="build"
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
cmake ..
if [ $? -ne 0 ]; then
    exit
fi
cmake --build .

