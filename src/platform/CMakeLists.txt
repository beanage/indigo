cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(PLATFORM_SOURCES
    ${DIR}/window.cpp
    ${DIR}/filesystem_unix.cpp
    
    PARENT_SCOPE)
    
set(PLATFORM_HEADERS
    ${DIR}/gl.hpp
    ${DIR}/window.hpp
    ${DIR}/filesystem.hpp
    
    PARENT_SCOPE)
