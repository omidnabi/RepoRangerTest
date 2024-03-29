@echo off
cd %~dp0

set BUILD_DIR=build

if exist %BUILD_DIR% (
    echo Removing existing build directory...
    rmdir /s /q %BUILD_DIR%
)

echo Creating new build directory...
mkdir %BUILD_DIR%
cd %BUILD_DIR%

echo Configuring and building project...
cmake .. && cmake --build .

pause

