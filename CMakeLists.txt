cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake) 

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

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
    contrib/stb)

add_executable(demo
    ${INDIGO_SOURCES}
    ${INDIGO_HEADERS}
    
    app/demo.cpp)

target_link_libraries(demo
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${SDL2_LIBRARY})

target_compile_features(demo
    PRIVATE cxx_range_for
    PRIVATE cxx_nullptr
    PRIVATE cxx_lambdas
    PRIVATE cxx_auto_type
    PRIVATE cxx_func_identifier
    PRIVATE cxx_right_angle_brackets
    PRIVATE cxx_override
    PRIVATE cxx_defaulted_functions
    PRIVATE cxx_variadic_templates)
