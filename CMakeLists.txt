cmake_minimum_required(VERSION 3.10)

project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RayTracer src/main.cpp)

target_include_directories(RayTracer PRIVATE ${CMAKE_SOURCE_DIR}/include)
