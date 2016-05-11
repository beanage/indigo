cmake_minimum_required(VERSION 3.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake) 

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wno-error=comment")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR})
    
add_definitions(-DGLM_FORCE_RADIANS)
    
add_subdirectory(src)

include_directories(
    src/
    contrib/stb
    contrib/json)

function(set_compile_features target)
    target_compile_features(${target}
        PRIVATE cxx_range_for
        PRIVATE cxx_nullptr
        PRIVATE cxx_lambdas
        PRIVATE cxx_auto_type
        PRIVATE cxx_func_identifier
        PRIVATE cxx_right_angle_brackets
        PRIVATE cxx_override
        PRIVATE cxx_defaulted_functions
        PRIVATE cxx_variadic_templates)
endfunction(set_compile_features)

function(set_warning_flags target)
    set_property(TARGET ${target} APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)
endfunction(set_warning_flags)

add_library(indigo STATIC
    ${INDIGO_SOURCES}
    ${INDIGO_HEADERS})

set_compile_features(indigo)
set_warning_flags(indigo)

target_link_libraries(indigo
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${SDL2_LIBRARY})

# TERRAIN

add_executable(terrain_test
    app/terrain_test.cpp)

set_compile_features(terrain_test)
set_warning_flags(terrain_test)

target_link_libraries(terrain_test
    indigo)
