#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "core/basic_geom.hpp"

#include <tuple>

namespace indigo {
class entity
{
public:
    virtual ~entity()
    {
    }

    virtual void update();
    virtual void render() const
    {
    }

    const glm::vec3& position() const;
    void position(const glm::vec3& pos);
    entity& move(glm::vec3 const& velocity);

    const glm::quat& rotation() const;
    void rotation(const glm::quat& rot);
    entity& turn_global(float angle, glm::vec3 axis);
    entity& turn_local(float angle, glm::vec3 axis);

    glm::mat4 orientation() const;
    glm::mat4 model() const;
    glm::mat4 model(float step) const;

    glm::vec3 forward() const;
    glm::vec3 up() const;
    glm::vec3 right() const;

    void look_at(const glm::vec3& target);

    aabb axis_aligned_bounding_box(glm::mat4 const& abs_transform) const
    {
        return aabb(glm::vec3(), glm::vec3());
    }
    virtual box bounding_box() const
    {
        return box();
    }
    virtual std::pair<bool, double> intersect(ray const& r) const
    {
        return std::pair<bool, double>(false, 0.0);
    }

protected:
    entity() = default;
    entity(const entity&) = default;

    virtual glm::mat4 build_model_martix(const glm::vec3& pos,
                                         const glm::quat& rot) const;

private:
    glm::vec3 position_, prev_position_;
    glm::quat rotation_, prev_rotation_;
};
}