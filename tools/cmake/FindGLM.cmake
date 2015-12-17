# Find GLM
#
# This module defines:
#     GLM_FOUND
#     GLM_INCLUDE_DIR

find_path(GLM_INCLUDE_DIR glm.hpp
	HINTS ENV GLMDIR
	PATH_SUFFIXES glm include/glm)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GLM DEFAULT_MSG
	GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)

