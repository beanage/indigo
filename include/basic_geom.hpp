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

        /// use the dims enum to access vertices in the verts() vector.
        /// eg. back bottom left corner would be verts()[back|bottom|left]
        enum dims {
            front = 0x0,
            bottom = 0x0,
            left = 0x0,
            top = 0x1,
            right = 0x2,
            back = 0x4
        };

        std::vector<glm::vec3> verts();

        bool intersect(aabb const& other, bool recurse = true) const;

        /// returns a vector that will move @other into @this. behaviour
        /// is undefined if @other is larger than @this on any dimension.
        /// returns a null vector if @other is already completely contained.
        glm::vec3 intersect_difference(aabb const& other) const;

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
}

#endif
