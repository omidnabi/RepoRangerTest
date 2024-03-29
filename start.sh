#!/bin/bash
./stop.sh
git pull

cd ./src
rm -rf build
mkdir build
cd build
cmake ..


cmake --build .


./ModifyConfig ../../config.json
