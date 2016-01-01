#include "camera.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#define CAM_MAX_FOV 110.f
#define CAM_MIN_FOV 0.f
#define CAM_DEF_FOV 50.f

#define CAM_DEF_NEAR .1f
#define CAM_DEF_FAR  10000.f

using namespace indigo;

static float clamp(float min, float value, float max)
{
	return std::min(max, std::max(min, value));
}

camera::camera()
	: fov_(CAM_DEF_FOV)
	, near_(CAM_DEF_NEAR)
	, far_(CAM_DEF_FAR)
{}

float camera::aspect_ratio() const
{
	return ratio_;
}

void camera::aspect_ratio(float value)
{
	ratio_ = value;
}

float camera::fov() const
{
	return fov_;
}

void camera::fov(float value)
{
	fov_ = clamp(CAM_MIN_FOV, value, CAM_MAX_FOV);
}

glm::mat4 camera::projection() const
{
	return glm::perspective(glm::radians(fov_), ratio_, near_, far_);
}

const glm::mat4& camera::view() const
{
	return model();
}
