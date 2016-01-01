#include "entity.h"
#include "basic_geom.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>

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

const glm::vec3& entity::position() const
{
    return position_;
}

void entity::position(const glm::vec3& pos)
{
    position_ = pos;
    model_ = build_model_martix();
}

entity &entity::move(const glm::vec3 &velocity)
{
    position(position() + velocity);
}

const glm::quat& entity::rotation() const
{
    return rotation_;
}

void entity::rotation(const glm::quat& rot)
{
    rotation_ = rot;
    model_ = build_model_martix();
}

entity &entity::turn(float angle, glm::vec3 axis)
{
    rotation_ = glm::rotate(rotation_, glm::radians(angle), axis);
    model_ = build_model_martix();
    return *this;
}

const glm::mat4& entity::model() const
{
    return model_;
}

glm::mat4 entity::orientation() const
{
    return glm::toMat4(rotation_);
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
    rotation_ = glm::quat_cast(glm::lookAt(glm::vec3(), target - position(), up()));
    model_ = build_model_martix();
}

glm::mat4 entity::build_model_martix()
{
    return glm::translate(glm::mat4(), position()) * orientation();
}
