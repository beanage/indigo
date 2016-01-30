
#ifndef __OCTNODE_H_INCLUDED__
#define __OCTNODE_H_INCLUDED__

#include <functional>
#include <memory>
#include <set>

#include "entity.h"
#include "basic_geom.hpp"

#define OCTNODE_NUM_ENTITY_SPLIT_THRESHOLD 10

namespace indigo
{
    class octnode;
    class renderer;
    typedef std::unique_ptr<octnode> unique_octnode;

    typedef glm::lowp_ivec3 octant_indices;

    class octnode
    {
        friend class renderer;

    public:
        void pull(entity const& ent);

    protected:
        inline bool is_leaf() const {return children[0][0][0].get() == nullptr;}

        /// this will be called by the renderer once every update
        void rebalance();

        aabb const geom;
        octant_indices indices;

        entity const* pick(ray const& r, glm::vec3 entry, double exit) const;

        octnode(aabb const& size, octant_indices pos = octant_indices(0,0,0));

    private:
        /// this function will return the octant for the given position.
        /// it will NOT check whether the position is actually inside the
        /// aabb! the returned vector has {zero,nonzero} as possible values
        /// on either dimension, with zero indicating (-) and nonzero indicating (+)
        octant_indices octant_indices_for_pos(glm::vec3 pos) const;

        /// might return nullptr if this is a leaf!
        unique_octnode const& octant_for_indices(octant_indices const& pos) const;

        void push(entity const& ent);
        void make_children();
        void consume_children();

        plane const lr, bt, fb;
        std::set<entity const*> entities;
        unique_octnode children[2][2][2];
        uint32_t entityrefs;
    };

    class root_octnode : public octnode
    {
    public:
        root_octnode(aabb const& size);

        glm::vec3 push(entity const& ent);
        entity const* pick(ray const& r) const;

    private:
        plane const left, right, top, bottom, front, back;
    };
}

#endif
