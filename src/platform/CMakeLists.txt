cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(PLATFORM_SOURCES
    ${DIR}/window.cpp
    ${DIR}/filesystem_unix.cpp
    ${DIR}/bitmap.cpp
    ${DIR}/bitmap_loader.cpp
    
    PARENT_SCOPE)
    
set(PLATFORM_HEADERS
    ${DIR}/gl.hpp
    ${DIR}/window.hpp
    ${DIR}/filesystem.hpp
    ${DIR}/bitmap.hpp
    ${DIR}/bitmap_loader.hpp
    ${DIR}/resource_manager.hpp
    
    PARENT_SCOPE)
