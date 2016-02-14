cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SCENE_SOURCES
    ${DIR}/camera.cpp
    ${DIR}/entity.cpp
    ${DIR}/mesh_entity.cpp
    ${DIR}/octnode.cpp
    
    PARENT_SCOPE)
    
set(SCENE_HEADERS
    ${DIR}/camera.hpp
    ${DIR}/entity.hpp
    ${DIR}/mesh_entity.hpp
    ${DIR}/octnode.hpp
    
    PARENT_SCOPE)
