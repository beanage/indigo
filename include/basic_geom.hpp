#ifndef __BASEGEOM_H_INCLUDED__
#define __BASEGEOM_H_INCLUDED__

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <vector>
#include <utility>
#include <ostream>

namespace indigo
{
    static const glm::vec3 x_axis(1.0,0,0);
    static const glm::vec3 y_axis(0,1.0,0);
    static const glm::vec3 z_axis(0,0,1.0);

    struct ray
    {
        ray(glm::vec3 pos, glm::vec3 direction);

        glm::vec3 const offset;
        glm::vec3 const direction;
    };

    struct plane
    {
        plane(glm::vec3 const& offset, glm::vec3 const& direction);
        double intersect(ray const& r, bool &valid) const;

        glm::vec3 const offset;
        glm::vec3 const direction;
    };

    struct aabb
    {
        aabb(glm::vec3 size, glm::vec3 front_bottom_right);

        bool intersect(aabb const& other) const;
        bool contains(double x, double y, double z) const;

        glm::vec3 const size;
        glm::vec3 const front_bottom_left; // corner that is closest to the origin if the bounding box was placed in the 1st octant (+,+,+)
        glm::vec3 const middle;
    };

    struct box
    {
        std::vector<glm::vec3> verts();
        aabb axis_aligned_bounding_box();

        glm::vec3 const size;
        glm::mat4 const transform;
    };

    /*

    std::ostream& operator<<(std::ostream& out, const aabb& r) {
            return out << "{size: " << r.size << ", pos: " << r.front_bottom_left << "}";
    }

    std::ostream& operator<<(std::ostream& out, const box& r) {
            return out << "{size: " << r.size << "}";
    }*/
}

#endif
