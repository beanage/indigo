cmake_minimum_required(VERSION 3.1)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(app)
add_subdirectory(model)
add_subdirectory(platform)
add_subdirectory(scene)
add_subdirectory(shader)
add_subdirectory(terrain)
add_subdirectory(util)
add_subdirectory(core)
add_subdirectory(mesh)

set(INDIGO_SOURCES
    ${APP_SOURCES}
    ${MODEL_SOURCES}
    ${PLATFORM_SOURCES}
    ${SCENE_SOURCES}
    ${SHADER_SOURCES}
    ${TERRAIN_SOURCES}
    ${UTIL_SOURCES}
    ${CORE_SOURCES}
    ${MESH_SOURCES}
    
    PARENT_SCOPE)
    
set(INDIGO_HEADERS
    ${APP_HEADERS}
    ${MODEL_HEADERS}
    ${PLATFORM_HEADERS}
    ${SCENE_HEADERS}
    ${SHADER_HEADERS}
    ${TERRAIN_HEADERS}
    ${UTIL_HEADERS}
    ${CORE_HEADERS}
    ${MESH_HEADERS}
    
    PARENT_SCOPE)
