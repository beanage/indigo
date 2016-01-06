#include "camera.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

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
    , ratio_(1.f)
    , mode_(view_mode::perspective)
{}

camera::view_mode camera::mode() const
{
    return mode_;
}

void camera::mode(view_mode m)
{
    mode_ = m;
}

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
    if (mode_ == view_mode::perspective)
        return glm::perspective(glm::radians(fov_), ratio_, near_, far_);
    return glm::ortho(-1.f, 1.f, 1.f, -1.f, near_, far_);
}

glm::mat4 camera::view() const
{
    return model();
}

glm::mat4 camera::view(float step) const
{
    return model(step);
}

void camera::turn(float yaw, float pitch)
{
    turn_global(yaw, glm::vec3(0, 1, 0));
    turn_local(pitch, glm::vec3(1, 0, 0));
}

glm::mat4 camera::build_model_martix(const glm::vec3& pos, const glm::quat& rot) const
{
    return glm::toMat4(rot) * glm::translate(glm::mat4(), -pos);
}
