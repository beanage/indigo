#pragma once

#include "model/bone.hpp"

namespace indigo {
class md5_bone : public bone
{
    friend class md5_loader;

public:
    md5_bone();

    std::string name() const;

    glm::vec3 position() const;
    void position(glm::vec3 pos);

    glm::quat rotation() const;
    void rotation(glm::quat rot);

    bone* parent();
    bone const* parent() const;

    glm::mat4 model() const;

private:
    std::string name_;
    glm::vec3 position_;
    glm::quat rotation_;
    md5_bone* parent_;
};
}
