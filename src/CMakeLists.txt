cmake_minimum_required(VERSION 3.14)
project(ModifyConfig)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include FetchContent module for downloading the JSON library
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.10.5 # Specify the version you want to use
)
FetchContent_MakeAvailable(json)

# Define the executable
add_executable(ModifyConfig main.cpp)

# Link the json library
target_link_libraries(ModifyConfig PRIVATE nlohmann_json::nlohmann_json)

