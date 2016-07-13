#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "core/basic_geom.hpp"

#include <memory>
#include <tuple>
#include <list>

namespace indigo
{
class renderer;
class entity;
typedef std::shared_ptr<entity> entity_shared_ptr;

class entity
{
    friend class renderer;
public:
    virtual ~entity() {}
    virtual void render(renderer&) const {}

    glm::vec3 position() const;
    void position(const glm::vec3& pos);
    entity& move(glm::vec3 const& velocity);

    glm::quat rotation() const;
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

    aabb axis_aligned_bounding_box(glm::mat4 const& abs_transform) const;

    virtual box bounding_box() const;
    virtual std::pair<bool, double> intersect(ray const& r) const;

    void add(const entity_shared_ptr& ent);
    void remove(const entity_shared_ptr& ent);

protected:
    entity() = default;
    entity(const entity&) = default;
    entity& operator=(const entity&) = default;

    void update_world_matrix(glm::mat4 const &parent, float time);
    virtual glm::mat4 build_model_matrix(const glm::vec3 &pos,
                                         const glm::quat &rot) const;

    glm::mat4 world_;

private:
    glm::vec3 position_, prev_position_;
    glm::quat rotation_, prev_rotation_;
    std::list<entity_shared_ptr> children_;
};
}
