#include "entity.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>

using namespace indigo;

void entity::update()
{
    prev_position_ = position_;
    prev_rotation_ = rotation_;
}

const glm::vec3& entity::position() const
{
    return position_;
}

void entity::position(const glm::vec3& pos)
{
    prev_position_ = position_ = pos;
}

entity& entity::move(const glm::vec3& velocity)
{
    if (prev_position_ == position_)
        prev_position_ = position_;
    position_ += velocity;
    return *this;
}

const glm::quat& entity::rotation() const
{
    return rotation_;
}

void entity::rotation(const glm::quat& rot)
{
    prev_rotation_ = rotation_ = rot;
}

entity& entity::turn_global(float angle, glm::vec3 axis)
{
    if (prev_rotation_ == rotation_)
        prev_rotation_ = rotation_;
    rotation_ = rotation_ * glm::rotate(glm::quat(), glm::radians(angle), axis);
    return *this;
}

entity& entity::turn_local(float angle, glm::vec3 axis)
{
    if (prev_rotation_ == rotation_)
        prev_rotation_ = rotation_;
    rotation_ = glm::rotate(glm::quat(), glm::radians(angle), axis) * rotation_;
    return *this;
}

glm::mat4 entity::model() const
{
    return build_model_martix(position_, rotation_);
}

glm::mat4 entity::model(float step) const
{
    glm::vec3 pos = glm::mix(prev_position_, position_, step);
    glm::quat rot = glm::slerp(prev_rotation_, rotation_, step);

    return build_model_martix(pos, rot);
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
    rotation(glm::quat_cast(glm::lookAt(glm::vec3(), target - position(), up())));
}

glm::mat4 entity::build_model_martix(const glm::vec3& pos,
                                     const glm::quat& rot) const
{
    return glm::translate(glm::mat4(), pos) * glm::toMat4(rot);
}
