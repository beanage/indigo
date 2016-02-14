cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(app)
add_subdirectory(model)
add_subdirectory(platform)
add_subdirectory(scene)
add_subdirectory(shader)
add_subdirectory(terrain)
add_subdirectory(util)

set(INDIGO_SOURCES
    ${DIR}/basic_geom.cpp
    ${DIR}/bitmap.cpp
    ${DIR}/bitmap_loader.cpp
    ${DIR}/material.cpp
    ${DIR}/texture.cpp
    
    ${APP_SOURCES}
    ${MODEL_SOURCES}
    ${PLATFORM_SOURCES}
    ${SCENE_SOURCES}
    ${SHADER_SOURCES}
    ${TERRAIN_SOURCES}
    ${UTIL_SOURCES}
    
    PARENT_SCOPE)
    
set(INDIGO_HEADERS
    ${DIR}/basic_geom.hpp
    ${DIR}/bitmap.hpp
    ${DIR}/bitmap_loader.hpp
    ${DIR}/colors.hpp
    ${DIR}/material.hpp
    ${DIR}/resource_manager.hpp
    ${DIR}/texture.hpp
    
    ${APP_HEADERS}
    ${MODEL_HEADERS}
    ${PLATFORM_HEADERS}
    ${SCENE_HEADERS}
    ${SHADER_HEADERS}
    ${TERRAIN_HEADERS}
    ${UTIL_HEADERS}
    
    PARENT_SCOPE)
