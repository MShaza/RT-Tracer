cmake_minimum_required(VERSION 3.28.3)
project(rt-scope VERSION 0.10 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")


add_definitions(-D_GNU_SOURCE)


add_compile_options(-Wall -Wextra)

find_package(Threads REQUIRED)


include_directories(includes) 

add_executable(rt-scope 
    src/main.cpp 
    src/worker.cpp 
    src/histogram.cpp
)

# 8. Link Libraries
target_link_libraries(rt-scope PRIVATE Threads::Threads)