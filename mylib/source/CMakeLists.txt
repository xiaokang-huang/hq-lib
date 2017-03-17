cmake_minimum_required(VERSION 3.7)
project(mylib)

set(SOURCE_FILES_MYLIB
        hq_memory_mgr.cpp
        hq_threadpool_fast.cpp
        linux_hq_thread.cpp
        hq_hash.cpp
        hq_mempool.cpp
        linux_hq_os.cpp
        linux_hq_thread_pool.cpp
        container/hq_binarynode.cpp
        container/hq_buffer.cpp
        container/hq_concurrentqueue.cpp
        container/hq_doublelist.cpp
        container/hq_node.cpp
        container/hq_quadnode.cpp
        math/hq_matrix.cpp
        math/hq_vertex.cpp
        math/hq_geometry.cpp
        util/hq_fsm.cpp
        image/hq_image.cpp
        )

add_library(mylib SHARED ${SOURCE_FILES_MYLIB})