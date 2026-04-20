cmake_minimum_required(VERSION 3.28.3)
project(rt-scope VERSION 0.10 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rt-scope)

target_sources(rt-scope PRIVATE src/main.cpp)