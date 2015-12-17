#include "camera.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#define CAM_MAX_FOV 110.f
#define CAM_MIN_FOV 0.f
#define CAM_DEF_FOV 50.f

#define CAM_DEF_NEAR .1f
#define CAM_DEF_FAR  100.f

using namespace indigo;

static float clamp(float min, float value, float max)
{
	return std::min(max, std::max(min, value));
}

static float normalize_angle(float value, float max)
{
	value = fmodf(value, 360.f);
	if (value < .0f)
		value += 360.f;
	if (value > max)
		value = max;
	return value;
}

camera::camera()
	: horizontal_(0.f)
	, vertical_(0.f)
	, fov_(CAM_DEF_FOV)
	, near_(CAM_DEF_NEAR)
	, far_(CAM_DEF_FAR)
{}

glm::vec3 camera::position() const
{
	return position_;
}

camera& camera::position(const glm::vec3& pos)
{
	position_ = pos;
	return *this;
}

float camera::aspect_ratio() const
{
	return ratio_;
}

camera& camera::aspect_ratio(float value)
{
	ratio_ = value;
	return *this;
}

float camera::fov() const
{
	return fov_;
}

camera& camera::fov(float value)
{
	fov_ = clamp(CAM_MIN_FOV, value, CAM_MAX_FOV);
	return *this;
}

float camera::horizontal() const
{
	return horizontal_;
}

camera& camera::horizontal(float horz)
{
	horizontal_ = horz;
	return *this;
}

float camera::vertical() const
{
	return vertical_;
}

camera& camera::vertical(float vert)
{
	vertical_ = vert;
	return *this;
}

glm::mat4 camera::orientation() const
{
	glm::mat4 result = glm::rotate(glm::mat4(), glm::radians(vertical_), glm::vec3(1.f, 0.f, 0.f));
	result = glm::rotate(result, glm::radians(horizontal_), glm::vec3(0.f, 1.f, 0.f));
	return result;
}

glm::mat4 camera::projection() const
{
	return glm::perspective(glm::radians(fov_), ratio_, near_, far_);
}

glm::mat4 camera::view() const
{
	return orientation() * glm::translate(glm::mat4(), -position());
}

glm::vec3 camera::forward() const
{
	return glm::vec3(glm::inverse(orientation()) * glm::vec4(0, 0, -1, 1));
}

glm::vec3 camera::up() const
{
	return glm::vec3(glm::inverse(orientation()) * glm::vec4(0, 1, 0, 1));
}

glm::vec3 camera::right() const
{
	return glm::vec3(glm::inverse(orientation()) * glm::vec4(1, 0, 0, 1));
}

camera& camera::look_at(const glm::vec3& target)
{
	glm::vec3 direction = glm::normalize(target - position());
	vertical(glm::radians(asinf(-direction.y)));
	horizontal(-glm::radians(atan2f(-direction.x, -direction.z)));
	return *this;
}
