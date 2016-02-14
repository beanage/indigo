cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(TERRAIN_SOURCES
    ${DIR}/terrain.cpp
    ${DIR}/terrain_cell.cpp
    ${DIR}/terrain_mesh.cpp
    
    PARENT_SCOPE)
    
set(TERRAIN_HEADERS
    ${DIR}/terrain.hpp
    ${DIR}/terrain_cell.hpp
    ${DIR}/terrain_mesh.hpp
    
    PARENT_SCOPE)
