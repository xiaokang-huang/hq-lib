cmake_minimum_required(VERSION 3.7)
project(engine)

set(CMAKE_CXX_STANDARD 11)

set(MYLIB_DIR ${PROJECT_SOURCE_DIR}/../mylib)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/internal_include ${MYLIB_DIR}/include /usr/local/include)

set(BUILD_DIRECTORY ${PROJECT_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH ${BUILD_DIRECTORY}/libs)

ADD_SUBDIRECTORY(${MYLIB_DIR}/source ${LIBRARY_OUTPUT_PATH})
ADD_SUBDIRECTORY(source)
ADD_SUBDIRECTORY(test)