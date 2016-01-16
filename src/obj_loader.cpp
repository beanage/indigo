#include "obj_loader.hpp"
#include "split.hpp"
#include "gl.hpp"

#include <vector>
#include <fstream>
#include <sstream>
#include <locale>
#include <memory>
#include <tuple>

using namespace indigo;

namespace
{
class obj_mesh : public mesh
{
public:
    static const unsigned int vertex_attribute_index   = 0;
    static const unsigned int texcoord_attribute_index = 1;
    static const unsigned int normal_attribute_index   = 2;

    struct vertex
    {
        glm::vec3 v;
        glm::vec3 vn;
        glm::vec2 vt;
    };

    obj_mesh()
        : vbo_(0), vao_(0)
    {}

    ~obj_mesh()
    {
        glDeleteBuffers(1, &vbo_);
        glDeleteVertexArrays(1, &vao_);
    }

    void render() const override
    {
        glBindVertexArray(vao_);
        glEnableVertexAttribArray(vertex_attribute_index);
        glEnableVertexAttribArray(texcoord_attribute_index);
        glEnableVertexAttribArray(normal_attribute_index);
        glDrawArrays(GL_TRIANGLES, 0, num_verts_);
        glBindVertexArray(0);
    }

    void upload() override
    {
        if (glIsBuffer(vbo_))
                throw std::runtime_error("[obj_mesh::upload] mesh is already online!");

        std::vector<vertex> data;
        data.resize(vertices_.size());

        // compile the separate vertex, uv and normal vector data
        auto pos_it = vertices_.begin();
        auto uv_it = uvs_.begin();
        auto norm_it = normals_.begin();
        for(vertex& vert : data) {
            vert.v = *pos_it;
            vert.vt = *uv_it;
            vert.vn = *norm_it;
            ++pos_it, ++uv_it, ++norm_it;
        }

        num_verts_ = data.size();

        glGenBuffers(1, &vbo_);
        glGenVertexArrays(1, &vao_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, num_verts_ * sizeof(vertex), data.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(vertex_attribute_index);
        glVertexAttribPointer(vertex_attribute_index, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(0));

        glEnableVertexAttribArray(normal_attribute_index);
        glVertexAttribPointer(normal_attribute_index, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(sizeof(glm::vec3)));

        glEnableVertexAttribArray(texcoord_attribute_index);
        glVertexAttribPointer(texcoord_attribute_index, 2, GL_FLOAT, GL_TRUE, sizeof(vertex), reinterpret_cast<void*>(sizeof(glm::vec3)+sizeof(glm::vec3)));
        glBindVertexArray(0);
    }

    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uvs_;

    gl_handle vao_;
    gl_handle vbo_;
    size_t num_verts_;
};
}

static int parse_face_indices_number(const std::string& str)
{
        if (!str.empty())
                return atoi(str.c_str());
        return 0;
}

static std::tuple<int, int, int> parse_face_indices(const std::string& input)
{
        auto parts = split(input, std::string("/"));

        return std::make_tuple(
                parse_face_indices_number(parts[0]),
                parse_face_indices_number(parts[1]),
                parse_face_indices_number(parts[2]));
}

obj_loader::obj_loader()
{}

bool obj_loader::can_load(const std::string& extension) const
{
    return extension == "obj";
}

std::shared_ptr<mesh> obj_loader::load(std::istream& stream)
{
    std::vector<glm::vec3> tmp_vertices;
    std::vector<glm::vec3> tmp_normals;
    std::vector<glm::vec2> tmp_uvs;

    std::vector<int> tmp_vertex_indices;
    std::vector<int> tmp_uv_indices;
    std::vector<int> tmp_normal_indices;

    if (stream.fail())
            throw std::runtime_error("[obj_loader] can not open file!");

    stream.imbue(std::locale("C"));

    for (std::string line; std::getline(stream, line);) {
        std::istringstream parts(line);

        std::string type;
        parts >> type;

        if (type == "v") {
            glm::vec3 vertex;
            parts >> vertex.x >> vertex.y >> vertex.z;

            tmp_vertices.push_back(std::move(vertex));
        }
        else if (type == "vt") {
            glm::vec2 uv;
            parts >> uv.x >> uv.y;

            tmp_uvs.push_back(std::move(uv));
        }
        else if (type == "vn") {
            glm::vec3 normal;
            parts >> normal.x >> normal.y >> normal.z;

            tmp_normals.push_back(std::move(normal));
        }
        else if (type == "f") {
            std::string vert[4];
            parts >> vert[0] >> vert[1] >> vert[2] >> vert[3];

            static const int vertex_order[] = {
                0, 1, 2, // first triangle
                0, 2, 3, // second triangle
            };

            bool is_quad = vert[3].size();
            int vertex_count = is_quad ? 6 : 3;

            for (int i = 0; i < vertex_count; ++i) {
                std::tuple<int, int, int> indices = parse_face_indices(vert[vertex_order[i]]);

                tmp_vertex_indices.push_back(std::get<0>(indices));
                tmp_uv_indices.push_back(std::get<1>(indices));
                tmp_normal_indices.push_back(std::get<2>(indices));
            }
        }
        else if (type == "mtllib") {
                // TODO: load material
        }
        else if (type == "usemtl") {
                // TODO: use model for the following faces
        }
    }

    if (tmp_vertex_indices.size() != tmp_uv_indices.size() ||
        tmp_vertex_indices.size() != tmp_normal_indices.size())
        throw std::runtime_error("[obj_loader] index sizes mismatching!");

    if (tmp_normals.empty())
        tmp_normals.push_back(glm::vec3());

    if (tmp_uvs.empty())
        tmp_uvs.push_back(glm::vec2());

	obj_mesh* result(new obj_mesh());

	for (auto index : tmp_vertex_indices)
        	result->vertices_.push_back(tmp_vertices[index-1]);

	for (auto index : tmp_uv_indices)
        	result->uvs_.push_back(tmp_uvs[index-1]);

	for (auto index : tmp_normal_indices)
        	result->normals_.push_back(tmp_normals[index-1]);

    return std::shared_ptr<mesh>(result);
}
