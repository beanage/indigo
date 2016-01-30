#include "basic_geom.hpp"
#include "glm/gtx/intersect.hpp"

#include <limits>

namespace indigo {

aabb::aabb(glm::vec3 size, glm::vec3 front_bottom_left) : size(size), front_bottom_left(front_bottom_left), middle(front_bottom_left + size/.5f) {}

std::vector<glm::vec3> aabb::verts()
{
    return {
        front_bottom_left, front_bottom_left + glm::vec3(0,size.y,0), front_bottom_left + glm::vec3(size.x,0,0), front_bottom_left + glm::vec3(size.x,size.y,0),
        front_bottom_left + glm::vec3(0,0,size.z), front_bottom_left + glm::vec3(0,size.y,size.z), front_bottom_left + glm::vec3(size.x,0,size.z), front_bottom_left + glm::vec3(size.x,size.y,size.z)
    };
}

bool aabb::intersect(const aabb &other, bool recurse) const
{
    return contains(other.front_bottom_left.x, other.front_bottom_left.y, other.front_bottom_left.z) ||
           contains(other.front_bottom_left.x + other.size.x, other.front_bottom_left.y, other.front_bottom_left.z) ||
           contains(other.front_bottom_left.x, other.front_bottom_left.y  + other.size.y, other.front_bottom_left.z) ||
           contains(other.front_bottom_left.x + other.size.x, other.front_bottom_left.y  + other.size.y, other.front_bottom_left.z) ||
           contains(other.front_bottom_left.x, other.front_bottom_left.y, other.front_bottom_left.z + other.size.z) ||
           contains(other.front_bottom_left.x + other.size.x, other.front_bottom_left.y, other.front_bottom_left.z + other.size.z) ||
           contains(other.front_bottom_left.x, other.front_bottom_left.y  + other.size.y, other.front_bottom_left.z + other.size.z) ||
           contains(other.front_bottom_left.x + other.size.x, other.front_bottom_left.y  + other.size.y, other.front_bottom_left.z + other.size.z) ||
            !recurse || other.intersect(*this, false);
}

glm::vec3 aabb::intersect_difference(const aabb& other) const
{
    glm::vec3 result;

    // iterate over all dimensions
    for(int i = 0; i < result.length(); ++i)
    {
        if(other.front_bottom_left[i] < front_bottom_left[i])
            result[i] = front_bottom_left[i] - other.front_bottom_left[i];
        else if(other.front_bottom_left[i] + other.size[i] > front_bottom_left[i] + size[i])
            result[i] = (front_bottom_left[i] + size[i]) - (other.front_bottom_left[i] + other.size[i]);
    }

    return result;
}

bool aabb::contains(double x, double y, double z) const
{
    return x >= front_bottom_left.x && x <= front_bottom_left.x + size.x &&
           y >= front_bottom_left.y && y <= front_bottom_left.y + size.y &&
           z >= front_bottom_left.z && z <= front_bottom_left.z + size.z;
}

std::vector<glm::vec3> box::verts()
{
    std::vector<glm::vec3> result = {
        glm::vec3(0,0,0), glm::vec3(0,size.y,0), glm::vec3(size.x,0,0), glm::vec3(size.x,size.y,0),
        glm::vec3(0,0,size.z), glm::vec3(0,size.y,size.z), glm::vec3(size.x,0,size.z), glm::vec3(size.x,size.y,size.z)
    };

    for(auto& vec : result)
        vec = glm::vec3(transform * glm::vec4(vec.x, vec.y, vec.z, 0.0));

    return result;
}

aabb box::axis_aligned_bounding_box()
{
    using std::numeric_limits<double>::quiet_NaN;

    double min_x(quiet_NaN()), min_y(quiet_NaN()), min_z(quiet_NaN()), max_x(quiet_NaN()), max_y(quiet_NaN()), max_z(quiet_NaN());

    for(auto const& vert : verts()) {
        if(vert.x < min_x || min_x == quiet_NaN())
            min_x = vert.x;
        if(vert.x > max_x || max_x == quiet_NaN())
            max_x = vert.x;
        if(vert.y < min_y || min_y == quiet_NaN())
            min_y = vert.y;
        if(vert.y > max_y || max_y == quiet_NaN())
            max_y = vert.y;
        if(vert.z < min_z || min_z == quiet_NaN())
            min_z = vert.z;
        if(vert.z > max_z || max_z == quiet_NaN())
            max_z = vert.z;
    }

    return aabb(glm::vec3(max_x-min_x, max_y-min_y, max_z-min_z), glm::vec3(min_x, min_y, min_z));
}

plane::plane(const glm::vec3 &offset, const glm::vec3 &direction) : offset(offset), direction(direction)
{
}

double plane::intersect(const ray &r, bool& valid) const
{
    float t = .0;
    valid = glm::intersectRayPlane(r.offset, r.direction, offset, direction, t);
    return t;
}

ray::ray(glm::vec3 pos, glm::vec3 direction) : offset(pos), direction(direction)
{
}

}
