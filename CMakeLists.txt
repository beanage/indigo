cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake) 

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR})

include_directories(
    include
    contrib/stb)

add_definitions(-DGLM_FORCE_RADIANS)

add_executable(demo
    src/window.cpp
    src/entity.cpp
    src/camera.cpp
    src/mesh_entity.cpp
    src/obj_loader.cpp
    src/mtl_loader.cpp
    src/texture.cpp
    src/shader.cpp
    src/program.cpp
    app/demo.cpp
    src/basic_geom.cpp
    src/octnode.cpp
    src/mesh.cpp
    src/application.cpp
    src/filesystem_unix.cpp)

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
