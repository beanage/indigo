cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CORE_SOURCES
    ${DIR}/basic_geom.cpp
    
    PARENT_SCOPE)
    
set(CORE_HEADERS
    ${DIR}/basic_geom.hpp
    
    PARENT_SCOPE)
