cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SHADER_SOURCES
    ${DIR}/program.cpp
    ${DIR}/shader.cpp
    
    PARENT_SCOPE)
    
set(SHADER_HEADERS
    ${DIR}/program.hpp
    ${DIR}/shader.hpp
    
    PARENT_SCOPE)
