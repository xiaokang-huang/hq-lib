cmake_minimum_required(VERSION 3.7)
project(mylib)

set(CMAKE_CXX_STANDARD 11)
include_directories(${PROJECT_SOURCE_DIR}/include /usr/local/include)

set(BUILD_DIRECTORY ${PROJECT_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH ${BUILD_DIRECTORY}/libs)


ADD_SUBDIRECTORY(source)
ADD_SUBDIRECTORY(test)