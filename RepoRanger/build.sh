#!/bin/bash

# Navigate to the project root directory if the script is run from another directory
cd "$(dirname "$0")"

# Define the build directory name
BUILD_DIR="build"

# Check if the build directory exists. If yes, remove it.
if [ -d "$BUILD_DIR" ]; then
    echo "Removing existing build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create a new build directory and navigate into it
echo "Creating new build directory..."
mkdir "$BUILD_DIR" && cd "$BUILD_DIR"

# Run CMake to configure the project and then build it
echo "Configuring and building project..."
cmake .. && cmake --build .

# Make the script executable with the following command:
# chmod +x build.sh


