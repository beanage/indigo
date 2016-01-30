#ifndef __INDIGO_DEBUG_HPP__
#define __INDIGO_DEBUG_HPP__

#include <iostream>
#include "basic_geom.hpp"

namespace indigo {
    std::ostream& operator<<(std::ostream& out, const glm::vec2& vec) {
        return out << "{x: " << vec.x << ", y: " << vec.y << "}";
    }

    std::ostream& operator<<(std::ostream& out, const glm::vec3& vec) {
        return out << "{x: " << vec.x << ", y: " << vec.y << ", z: " << vec.z << "}";
    }

    std::ostream& operator<<(std::ostream& out, const glm::vec4& vec) {
        return out << "{x: " << vec.x << ", y: " << vec.y << ", z: " << vec.z << ", w:" << vec.w << "}";
    }

    std::ostream& operator<<(std::ostream& out, const glm::quat& q) {
        return out << "{x: " << q.x << ", y: " << q.y << ", z: " << q.z << ", w: " << q.w <<"}";
    }

    std::ostream& operator<<(std::ostream& out, const aabb& r) {
        return out << "{size: " << r.size << ", pos: " << r.front_bottom_left << "}";
    }

    std::ostream& operator<<(std::ostream& out, const box& r) {
        return out << "{size: " << r.size << "}";
    }
}

#endif
