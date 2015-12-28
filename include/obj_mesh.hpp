#ifndef __OBJ_MESH_HPP_INCLUDED__
#define __OBJ_MESH_HPP_INCLUDED__

#include "mesh.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace indigo
{
	class obj_mesh : public mesh
	{
		friend class obj_loader;
	public:
		void upload();
		void render();

	private:
		obj_mesh();

		unsigned int vao_;
		unsigned int vbo_;

		std::vector<glm::vec3> vertices_;
		std::vector<glm::vec2> uvs_;
		std::vector<glm::vec2> normals_;
	};
}

#endif
