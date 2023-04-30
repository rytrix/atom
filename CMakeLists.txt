set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(_DEBUG)
endif()

cmake_minimum_required(VERSION 3.22.0)

if (BUILD_TESTS STREQUAL "ON")
    project(atom_test)
    add_executable(${PROJECT_NAME} main.cpp)
    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
    add_subdirectory(atom)
    target_link_libraries(${PROJECT_NAME} atom)
    target_include_directories(${PROJECT_NAME} PUBLIC atom)
else()
    add_subdirectory(atom)
endif()




