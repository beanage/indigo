#ifndef __MESH_HPP_INCLUDED__
#define __MESH_HPP_INCLUDED__

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "gl.hpp"

namespace indigo
{
    class mesh
    {
    public:
        virtual ~mesh() {}

        virtual void render() const = 0;
        virtual void upload() = 0;
    };
}

#endif
