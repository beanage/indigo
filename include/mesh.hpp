#ifndef __MESH_HPP_INCLUDED__
#define __MESH_HPP_INCLUDED__

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "gl.hpp"

namespace indigo
{
    struct vertex
    {
        glm::vec3 v;
        glm::vec2 vn;
        glm::vec2 vt;
    };

    class mesh
    {
    public:
        static const unsigned int vertex_attribute_index = 0;
        static const unsigned int texcoord_attribute_index = 1;
        static const unsigned int normal_attribute_index = 2;

        mesh();
        virtual ~mesh() {}
        virtual void render() const;
        virtual void upload() = 0;

    protected:
        virtual void upload(std::vector<vertex> verts);

        gl_handle vao_;
        gl_handle vbo_;
        size_t num_verts;
    };
}

#endif
