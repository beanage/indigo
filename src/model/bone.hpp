#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace indigo {
class bone
{
public:
    virtual ~bone() {}

    virtual std::string name() const = 0;

    virtual glm::vec3 position() const = 0;
    virtual void position(glm::vec3 pos) = 0;

    virtual glm::quat rotation() const = 0;
    virtual void rotation(glm::quat rot) = 0;

    virtual bone* parent() = 0;
    virtual bone const* parent() const = 0;

protected:
    bone() {}
};
}
