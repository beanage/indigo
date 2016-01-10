#ifndef __MD5_MESH_HPP_INCLUDED__
#define __MD5_MESH_HPP_INCLUDED__

#include "mesh.hpp"
#include <string>

namespace indigo
{
class md5_mesh : public mesh
{
    friend class md5_loader;
public:
	struct vertex
	{
		glm::vec2 uv;
		float start_weight;
		float count_weight;
	};

	struct weight
	{
		float joint;
		float bias;
		glm::vec3 position;
	};

    md5_mesh();
    ~md5_mesh();

	virtual void render() const;
	virtual void upload();

private:
	void upload_vertices();
	void upload_weights();

    std::string material_; // TODO: material instance

	gl_handle vao_;
	gl_handle vbo_;
    gl_handle tbo_;
    gl_handle tbo_tex_;

	size_t num_verts_;
    size_t num_weights_;

    std::vector<vertex> vertices_;
    std::vector<weight> weights_;
};

}

#endif
