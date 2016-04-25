#include "octnode.hpp"
#include "glm/gtx/intersect.hpp"

#include <algorithm>
#include <array>
#include <limits>

namespace indigo {

/************************ octnode **********************/

struct octnode_plane_isect
{
    octnode_plane_isect(ray const& r, plane const& p,
                        glm::lowp_ivec3 step = glm::lowp_ivec3(0, 0, 0))
        : step(step)
    {
        bool valid = false;
        t = p.intersect(r, valid);
        if (valid) {
            pos = r.offset + r.direction * t;
            float s = 0;
            valid = glm::intersectRayPlane(r.offset, p.direction, p.offset,
                                           p.direction, s);
            if (s < 0)
                step *= -1;
        }
    }

    glm::lowp_ivec3 step;
    glm::vec3 pos;
    float t;

    bool operator<(octnode_plane_isect const& other) const
    {
        return t < other.t;
    }
};

octnode::octnode(const aabb& geom, octant_indices pos)
    : geom(geom), indices(pos), lr(geom.middle, x_axis), bt(geom.middle, y_axis), fb(geom.middle, z_axis)
{
}

bool octnode::push(const entity& ent, glm::mat4 const& abs_transform)
{
    if (!geom.intersect(ent.axis_aligned_bounding_box(abs_transform)))
        return false;

    if (is_leaf()) {
        entities.push_back(&ent);
        if (entities.size() > OCTNODE_NUM_ENTITY_SPLIT_THRESHOLD) {
            make_children();
            for (auto const& e : entities)
                push(*e, abs_transform);
            entities.clear();
        }
        return true;
    } else
        for (auto const& z : children)
            for (auto const& y : z)
                for (auto const& x : y)
                    if (x->push(ent, abs_transform))
                        return true;

    throw std::runtime_error("Failed to insert entity into oct for no reason.");
}

entity const* octnode::pick(const ray& r, glm::vec3 entry, double exit) const
{
    if (is_leaf()) {
        entity const* closest_entity = nullptr;
        double best_t = std::numeric_limits<double>::infinity();
        for (entity const* ent : entities) {
            auto result = ent->intersect(r);
            if (result.first && result.second < best_t && result.second > 0) {
                best_t = result.second;
                closest_entity = ent;
            }
        }
        return closest_entity;
    } else {
        // determine the succession of intersections
        std::array<octnode_plane_isect, 3> inner_isects = {
            octnode_plane_isect(r, lr, glm::lowp_ivec3(1, 0, 0)),
            octnode_plane_isect(r, bt, glm::lowp_ivec3(0, 1, 0)),
            octnode_plane_isect(r, fb, glm::lowp_ivec3(0, 0, 1))};
        std::sort(inner_isects.begin(), inner_isects.end());

        octnode const* current_node =
            octant_for_indices(octant_indices_for_pos(entry)).get();

        // move forward to the first positive intersection
        auto isect_it = inner_isects.begin();
        for (; isect_it != inner_isects.end() && (*isect_it).t < 0; ++isect_it)
            ;

        do {
            // determine the current exit
            double current_exit = exit;
            if (isect_it != inner_isects.end()) {
                // skip intersections beyond the exit
                if ((*isect_it).t > exit)
                    isect_it = inner_isects.end();
                else
                    current_exit = (*isect_it).t;
            }

            // pick in the current node
            entity const* result = current_node->pick(r, entry, current_exit);
            if (result)
                return result;

            // determine next node and advance the intersection iterator
            if (isect_it != inner_isects.end()) {
                octnode_plane_isect const& isect = *isect_it;
                entry = isect.pos;
                current_node =
                    octant_for_indices(current_node->indices + isect.step).get();
                ++isect_it;
            } else
                current_node = nullptr;
        } while (current_node != nullptr);
    }
    return nullptr;
}

void octnode::make_children()
{
    glm::vec3 pos = geom.front_bottom_left;
    glm::vec3 size = geom.size / 2.0f;
    for (int x = 0; x < 2; ++x) {
        pos.y = 0;
        for (int y = 0; y < 2; ++y) {
            pos.z = 0;
            for (int z = 0; z < 2; ++z) {
                children[z][y][x] = unique_octnode(new octnode(aabb(size, pos)));
                pos.z += size.z;
            }
            pos.y += size.y;
        }
        pos.x += size.x;
    }
}

octant_indices octnode::octant_indices_for_pos(glm::vec3 pos) const
{
    octant_indices result(0, 0, 0);

    if (pos.x > geom.middle.x)
        result.x = 1;
    if (pos.y > geom.middle.y)
        result.y = 1;
    if (pos.z > geom.middle.z)
        result.z = 1;

    return result;
}

const unique_octnode&
octnode::octant_for_indices(const octant_indices& pos) const
{
    return children[std::max(pos.z, 1)][std::max(pos.y, 1)][std::max(pos.x, 1)];
}

/************************ root_octnode **********************/

root_octnode::root_octnode(const aabb& size)
    : octnode(size), left(geom.front_bottom_left, x_axis), right(geom.front_bottom_left + glm::vec3(geom.size.x, 0, 0), x_axis), bottom(geom.front_bottom_left, y_axis), top(geom.front_bottom_left + glm::vec3(0, geom.size.y, 0), y_axis), front(geom.front_bottom_left, z_axis), back(geom.front_bottom_left + glm::vec3(0, 0, geom.size.z), z_axis)
{
}

const entity* root_octnode::pick(const ray& r) const
{
    std::array<octnode_plane_isect, 6> bounding_isects = {
        octnode_plane_isect(r, top), octnode_plane_isect(r, bottom),
        octnode_plane_isect(r, left), octnode_plane_isect(r, right),
        octnode_plane_isect(r, front), octnode_plane_isect(r, back)};

    std::sort(bounding_isects.begin(), bounding_isects.end());
    double first_positive_intersection = .0;
    for (octnode_plane_isect const& isect : bounding_isects)
        if (isect.t >= 0) {
            first_positive_intersection = isect.t;
            break;
        }

    return octnode::pick(r, r.offset, first_positive_intersection);
}
}
