cmake_minimum_required(VERSION 3.17)
project(kmeans_l)

set(CMAKE_CXX_STANDARD 14)

add_executable(kmeans_l src/main.cpp)

target_link_libraries(kmeans_l -lOpenGL32 -lfreeGLUT)