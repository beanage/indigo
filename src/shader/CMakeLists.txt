cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SHADER_SOURCES
    ${DIR}/basic_program.cpp
    ${DIR}/default_program.cpp
    ${DIR}/shader.cpp
    ${DIR}/shader_loader.cpp
    PARENT_SCOPE)
    
set(SHADER_HEADERS
    ${DIR}/basic_program.hpp
    ${DIR}/default_program.hpp
    ${DIR}/shader.hpp
    ${DIR}/shader_loader.hpp
    PARENT_SCOPE)
