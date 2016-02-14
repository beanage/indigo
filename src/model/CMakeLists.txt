cmake_minimum_required(VERSION 3.1)

set(DIR ${DIR}/model)

add_subdirectory(md5)
add_subdirectory(obj)

set(MODEL_SOURCES
    ${MD5_SOURCES}
    ${OBJ_SOURCES}
    
    PARENT_SCOPE)
    
set(MODEL_HEADERS
    ${DIR}/mesh.hpp
    ${DIR}/model.hpp
    ${DIR}/bone.hpp
    
    ${MD5_HEADERS}
    ${OBJ_HEADERS}
    
    PARENT_SCOPE)
