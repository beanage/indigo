cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(UTIL_SOURCES
    PARENT_SCOPE)
    
set(UTIL_HEADERS
    ${DIR}/log.hpp
    ${DIR}/rect.hpp
    ${DIR}/split.hpp
    ${DIR}/switch_t.hpp
    ${DIR}/debug.hpp
    
    PARENT_SCOPE)
