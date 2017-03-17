cmake_minimum_required(VERSION 3.7)
project(mylib)

find_package(Threads)
find_package(CppUnit)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN/../libs")

set(SOURCE_FILES_TEST
        test_container.cpp
        test_image.cpp
        test_math.cpp
        test_thread.cpp
        test_threadpool_fast.cpp
        test_hash.cpp
        test_main.cpp
        test_memorymgr.cpp
        test_threadpool.cpp
        )

add_executable(test_mylib ${SOURCE_FILES_TEST})

target_link_libraries(test_mylib mylib)
target_link_libraries(test_mylib ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(test_mylib ${CPPUNIT_LIBRARIES})
