set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(_DEBUG)
endif()

cmake_minimum_required(VERSION 3.22.0)

project(atom)

add_library(${PROJECT_NAME} semaphore.cpp fence.cpp threadpool.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_precompile_headers(${PROJECT_NAME} PUBLIC pch.hpp)

