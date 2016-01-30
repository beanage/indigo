#ifndef __ENTITY_H_INCLUDED__
#define __ENTITY_H_INCLUDED__

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "basic_geom.hpp"

#include <tuple>
#include <memory>
#include <list>

namespace indigo
{
class entity;
class octnode;
typedef std::shared_ptr<entity> entity_shared_ptr;

class entity : protected std::enable_shared_from_this<entity>
{
public:
    entity(entity_shared_ptr parent = nullptr);

    void parent(entity_shared_ptr new_parent);
    entity_shared_ptr parent();
    void remove();

    virtual ~entity() {}
    virtual void update();
    virtual void render() const {}

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

    aabb axis_aligned_bounding_box(glm::mat4 const& abs_transform) const {return aabb(glm::vec3(),glm::vec3());}
    virtual box bounding_box() const {return box();}
    virtual std::pair<bool, double> intersect(ray const& r) const {return std::pair<bool, double>(false, 0.0);}

protected:
    entity(const entity&) = default;

    virtual glm::mat4 build_model_matrix(const glm::vec3& pos, const glm::quat& rot) const;

    void update_world_matrix_and_octnode(const glm::mat4& parent);

private:
    glm::vec3 position_, prev_position_;
    glm::quat rotation_, prev_rotation_;

    entity* parent_;
    std::list<entity_shared_ptr> children_;
    glm::mat4 world_transform;
    octnode* oct;
};
}

#endif
