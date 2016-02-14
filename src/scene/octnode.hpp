#ifndef __OCTNODE_H_INCLUDED__
#define __OCTNODE_H_INCLUDED__

#include <functional>
#include <memory>
#include <list>

#include "scene/entity.hpp"
#include "basic_geom.hpp"

#define OCTNODE_NUM_ENTITY_SPLIT_THRESHOLD 10

namespace indigo {
class octnode;
typedef std::unique_ptr<octnode> unique_octnode;

typedef glm::lowp_ivec3 octant_indices;

class octnode
{
public:
    bool push(entity const& ent, const glm::mat4& abs_transform);

protected:
    inline bool is_leaf() const
    {
        return children[0][0][0].get() == nullptr;
    }

    aabb const geom;
    octant_indices indices;

    entity const* pick(ray const& r, glm::vec3 entry, double exit) const;

    octnode(aabb const& size, octant_indices pos = octant_indices(0, 0, 0));

private:
    /// this function will return the octant for the given position.
    /// it will NOT check whether the position is actually inside the
    /// aabb! the returned vector has {zero,nonzero} as possible values
    /// on either dimension, with zero indicating (-) and nonzero indicating (+)
    octant_indices octant_indices_for_pos(glm::vec3 pos) const;

    /// might return nullptr if this is a leaf!
    unique_octnode const& octant_for_indices(octant_indices const& pos) const;

    void make_children();

    plane const lr, bt, fb;
    std::list<entity const*> entities;
    unique_octnode children[2][2][2];
};

class root_octnode : public octnode
{
public:
    root_octnode(aabb const& size);

    entity const* pick(ray const& r) const;

private:
    plane const left, right, top, bottom, front, back;
};
}

#endif
