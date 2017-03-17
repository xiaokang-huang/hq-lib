cmake_minimum_required(VERSION 3.7)
project(engine)

find_package(Threads)
find_package(CppUnit)
find_package(OpenGL)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN/../libs ")

set(SOURCE_FILES_TEST main.cpp)

add_executable(test_engine ${SOURCE_FILES_TEST})

target_link_libraries(test_engine engine)
target_link_libraries(test_engine mylib)
target_link_libraries(test_engine ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(test_engine ${CPPUNIT_LIBRARIES})
target_link_libraries(test_engine ${OPENGL_gl_LIBRARY})
