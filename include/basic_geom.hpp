#ifndef __AABB_H_INCLUDED__
#define __AABB_H_INCLUDED__

#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"

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
        glm::vec3 intersect(ray const& r, bool &valid);

        glm::vec3 const offset;
        glm::vec3 const direction;
    };

    struct aabb
    {
        aabb(glm::vec3 size, glm::vec3 front_bottom_right);

        bool intersect(aabb const& other);
        bool contains(double x, double y, double z);

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

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const aabb& r) {
            return out << "{size: [" << r.size.x << ", " << r.size.y << ", " << r.size.z << "], pos: [" << r.front_bottom_left.x << ", " << r.front_bottom_left.y << ", " << r.front_bottom_left.z << "]}";
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const box& r) {
            return out << "{size: [" << r.size.x << ", " << r.size.y << ", " << r.size.z << "]}";
    }
}

#endif
