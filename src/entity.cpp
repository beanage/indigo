#include "entity.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace indigo;

static float normalize_angle(float value, float max)
{
	value = fmodf(value, 360.f);
	if (value < .0f)
		value += 360.f;
	if (value > max)
		value = max;
	return value;
}

static glm::mat4 build_model_matrix(const entity& e)
{
	return e.orientation() * glm::translate(glm::mat4(), -e.position());
}

entity::entity()
{}

const glm::vec3& entity::position() const
{
	return position_;
}

void entity::position(const glm::vec3& pos)
{
	position_ = pos;
	model_ = build_model_matrix(*this);
}

const glm::vec3& entity::rotation() const
{
	return rotation_;
}

void entity::rotation(const glm::vec3& rot)
{
	rotation_ = rot;
	model_ = build_model_matrix(*this);
}

const glm::mat4& entity::model() const
{
	return model_;
}

glm::mat4 entity::orientation() const
{
	glm::mat4 result = glm::rotate(glm::mat4(), glm::radians(rotation_.z), glm::vec3(1.f, 0.f, 0.f));
	result = glm::rotate(result, glm::radians(rotation_.x), glm::vec3(0.f, 1.f, 0.f));
	return result;
}

glm::vec3 entity::forward() const
{
	return glm::vec3(glm::inverse(orientation()) * glm::vec4(0, 0, -1, 1));
}

glm::vec3 entity::up() const
{
	return glm::vec3(glm::inverse(orientation()) * glm::vec4(0, 1, 0, 1));
}

glm::vec3 entity::right() const
{
	return glm::vec3(glm::inverse(orientation()) * glm::vec4(1, 0, 0, 1));
}

void entity::look_at(const glm::vec3& target)
{
	glm::vec3 direction = glm::normalize(target - position());
	rotation_.x = glm::radians(asinf(-direction.y));
	rotation_.z = -glm::radians(atan2f(-direction.x, -direction.z));
}
