#include "basic_geom.hpp"
#include "glm/gtx/intersect.hpp"

#include <limits>

namespace indigo {

aabb::aabb(glm::vec3 size, glm::vec3 front_bottom_left)
    : size(size), front_bottom_left(front_bottom_left), middle(front_bottom_left + size / .5f)
{
}

bool aabb::intersect(const aabb& other) const
{
    return contains(other.front_bottom_left.x, other.front_bottom_left.y,
                    other.front_bottom_left.z) ||
           contains(other.front_bottom_left.x + other.size.x,
                    other.front_bottom_left.y, other.front_bottom_left.z) ||
           contains(other.front_bottom_left.x,
                    other.front_bottom_left.y + other.size.y,
                    other.front_bottom_left.z) ||
           contains(other.front_bottom_left.x + other.size.x,
                    other.front_bottom_left.y + other.size.y,
                    other.front_bottom_left.z) ||
           contains(other.front_bottom_left.x, other.front_bottom_left.y,
                    other.front_bottom_left.z + other.size.z) ||
           contains(other.front_bottom_left.x + other.size.x,
                    other.front_bottom_left.y,
                    other.front_bottom_left.z + other.size.z) ||
           contains(other.front_bottom_left.x,
                    other.front_bottom_left.y + other.size.y,
                    other.front_bottom_left.z + other.size.z) ||
           contains(other.front_bottom_left.x + other.size.x,
                    other.front_bottom_left.y + other.size.y,
                    other.front_bottom_left.z + other.size.z) ||
           other.intersect(*this);
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
        glm::vec3(0, 0, 0), glm::vec3(0, size.y, 0),
        glm::vec3(size.x, 0, 0), glm::vec3(size.x, size.y, 0),
        glm::vec3(0, 0, size.z), glm::vec3(0, size.y, size.z),
        glm::vec3(size.x, 0, size.z), glm::vec3(size.x, size.y, size.z)};

    for (auto& vec : result)
        vec = glm::vec3(transform * glm::vec4(vec.x, vec.y, vec.z, 0.0));

    return result;
}

aabb box::axis_aligned_bounding_box()
{
    double nan = std::numeric_limits<double>::quiet_NaN();
    double min_x(nan), min_y(nan), min_z(nan), max_x(nan), max_y(nan), max_z(nan);

    for (auto const& vert : verts()) {
        if (vert.x < min_x || min_x == nan)
            min_x = vert.x;
        if (vert.x > max_x || max_x == nan)
            max_x = vert.x;
        if (vert.y < min_y || min_y == nan)
            min_y = vert.y;
        if (vert.y > max_y || max_y == nan)
            max_y = vert.y;
        if (vert.z < min_z || min_z == nan)
            min_z = vert.z;
        if (vert.z > max_z || max_z == nan)
            max_z = vert.z;
    }

    return aabb(glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z),
                glm::vec3(min_x, min_y, min_z));
}

plane::plane(const glm::vec3& offset, const glm::vec3& direction)
    : offset(offset), direction(direction)
{
}

double plane::intersect(const ray& r, bool& valid) const
{
    float t = .0;
    valid = glm::intersectRayPlane(r.offset, r.direction, offset, direction, t);
    return t;
}

ray::ray(glm::vec3 pos, glm::vec3 direction)
    : offset(pos), direction(direction)
{
}
}
