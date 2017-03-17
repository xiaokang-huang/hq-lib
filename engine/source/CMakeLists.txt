cmake_minimum_required(VERSION 3.7)
project(engine)

set(SOURCE_FILES_ENGINE
        scene/hq_scenenode.cpp
        hq_engine.cpp
        mesh/hq_staticmesh.cpp
        mesh/hq_meshbase.cpp
        rendersystem/linux_hq_rendertarget.cpp
        rendersystem/linux_hq_texture.cpp
        rendersystem/linux_hq_rendersystem.cpp
        linux_hq_window.cpp
        resource/hq_meshmanager.cpp
        )

add_library(engine SHARED ${SOURCE_FILES_ENGINE})

target_link_libraries(engine mylib)