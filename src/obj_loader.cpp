#include "obj_loader.hpp"
#include "obj_mesh.hpp"
#include "split.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <locale>
#include <memory>
#include <tuple>

using namespace indigo;

struct face
{
	size_t size;
	int vertices[4];
	int uvs[4];
	int normals[4];
};

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
		parse_face_indices_number(parts[2])
	);
}

obj_loader::obj_loader()
{}

std::unique_ptr<obj_mesh> obj_loader::load(const std::string& filename)
{
	std::vector<glm::vec3> tmp_vertices;
	std::vector<glm::vec2> tmp_uvs;
	std::vector<glm::vec2> tmp_normals;

	std::vector<int> tmp_vertex_indices;
	std::vector<int> tmp_uv_indices;
	std::vector<int> tmp_normal_indices;

	std::ifstream file(filename, std::ifstream::in);
	if (file.fail())
		throw std::runtime_error("[obj_loader] can not open file!");
	
	file.imbue(std::locale("C"));

	for (std::string line; std::getline(file, line);) {
		std::istringstream parts(line);
		
		std::string type;
		parts >> type;

		if (type == "v") {
			glm::vec3 vertex;
			parts >> vertex.x >> vertex.y >> vertex.z;

			tmp_vertices.push_back(std::move(vertex));
		}
		else
		if (type == "vt") {
			glm::vec2 uv;
			parts >> uv.x >> uv.y;

			tmp_uvs.push_back(std::move(uv));
		}
		else
		if (type == "vn") {
			glm::vec2 normal;
			parts >> normal.x >> normal.y;

			tmp_normals.push_back(std::move(normal));
		}
		else
		if (type == "f") {
			std::string vert[3];
			parts >> vert[0] >> vert[1] >> vert[2];

			// TODO: Obj can also store quads! e.g. vert[4]
			for (int i = 0; i < 3; ++i) {
				std::tuple<int, int, int> indices = parse_face_indices(vert[i]);

				tmp_vertex_indices.push_back(std::get<0>(indices));
				tmp_uv_indices.push_back(std::get<1>(indices));
				tmp_normal_indices.push_back(std::get<2>(indices));
			}
		}
	}

	file.close();
	//if (file.fail())
	//	throw std::runtime_error("[obj_loader] error closing file!");

	if (tmp_vertex_indices.size() != tmp_uv_indices.size() ||
	    tmp_vertex_indices.size() != tmp_normal_indices.size())
		throw std::runtime_error("[obj_loader] index sizes mismatching!");

	// TODO: remove this two lines.
	tmp_normals.push_back(glm::vec2());
	tmp_uvs.push_back(glm::vec2());

	std::unique_ptr<obj_mesh> mesh(new obj_mesh);

	for (auto index : tmp_vertex_indices)
		mesh->vertices_.push_back(tmp_vertices[index-1]);

	for (auto index : tmp_uv_indices)
		mesh->uvs_.push_back(tmp_uvs[index-1]);

	for (auto index : tmp_normal_indices)
		mesh->normals_.push_back(tmp_normals[index-1]);

	return mesh;
}
