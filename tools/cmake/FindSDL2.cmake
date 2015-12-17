# Find SDL2
#
# This module defines:
#     SDL2_FOUND
#     SDL2_LIBRARY
#     SDL2_INCLUDE_DIR

find_path(SDL2_INCLUDE_DIR SDL.h
	HINTS ENV SDLDIR
	PATH_SUFFIXES SDL2 include/SDL2)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(VC_LIB_PATH_SUFFIX lib/x64)
else()
	set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

find_library(SDL2_LIBRARY
	NAMES SDL2
	HINTS ENV SDLDIR
	PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SDL2 DEFAULT_MSG
                                  SDL2_LIBRARY SDL2_INCLUDE_DIR)

mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY)

