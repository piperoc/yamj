cmake_minimum_required(VERSION 3.20)
project(yamj LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} WIN32 src/main.cpp)

# Link Windows libraries
target_link_libraries(${PROJECT_NAME} PRIVATE user32 shell32 comctl32)
