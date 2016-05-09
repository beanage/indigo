cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SCENE_SOURCES
    ${DIR}/camera.cpp
    ${DIR}/entity.cpp
    ${DIR}/mesh_entity.cpp
    ${DIR}/octnode.cpp
    ${DIR}/scene.cpp
    ${DIR}/renderer.cpp
    ${DIR}/group_entity.cpp
    ${DIR}/basic_light.cpp
    PARENT_SCOPE)
    
set(SCENE_HEADERS
    ${DIR}/camera.hpp
    ${DIR}/entity.hpp
    ${DIR}/mesh_entity.hpp
    ${DIR}/scene.hpp
    ${DIR}/renderer.hpp
    ${DIR}/group_entity.hpp
    ${DIR}/octnode.hpp
    ${DIR}/basic_light.hpp
    PARENT_SCOPE)
