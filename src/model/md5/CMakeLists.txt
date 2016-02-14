cmake_minimum_required(VERSION 3.1)

set(DIR ${DIR}/md5)

set(MD5_SOURCES
    ${DIR}/md5_bone.cpp
    ${DIR}/md5_loader.cpp
    ${DIR}/md5_mesh.cpp
    ${DIR}/md5_model.cpp
    
    PARENT_SCOPE)
    
set(MD5_HEADERS
    ${DIR}/md5_bone.hpp
    ${DIR}/md5_loader.hpp
    ${DIR}/md5_mesh.hpp
    ${DIR}/md5_model.hpp
    
    PARENT_SCOPE)
