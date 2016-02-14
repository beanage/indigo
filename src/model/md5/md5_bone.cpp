#include "md5_bone.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace indigo;

md5_bone::md5_bone()
    : parent_(nullptr)
{
}

std::string md5_bone::name() const
{
    return name_;
}

glm::vec3 md5_bone::position() const
{
    return position_;
}

void md5_bone::position(glm::vec3 pos)
{
    position_ = pos;
}

glm::quat md5_bone::rotation() const
{
    return rotation_;
}

void md5_bone::rotation(glm::quat rot)
{
    rotation_ = rot;
}

bone* md5_bone::parent()
{
    return parent_;
}

bone const* md5_bone::parent() const
{
    return parent_;
}

glm::mat4 md5_bone::model() const
{
    return glm::translate(glm::mat4(1), position_) * glm::toMat4(rotation_);
}
