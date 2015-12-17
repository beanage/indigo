cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake) 

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR})

include_directories(
	include
	contrib/stb)

add_executable(demo
	src/window.cpp
	src/camera.cpp
	app/demo.cpp)

target_link_libraries(demo
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARY}
	${SDL2_LIBRARY})

target_compile_features(demo
	PRIVATE cxx_range_for)
