#ifndef __MESH_HPP_INCLUDED__
#define __MESH_HPP_INCLUDED__

namespace indigo
{
	class mesh
	{
	public:
		static const unsigned int vertex_attribute_index = 0;
		static const unsigned int texcoord_attribute_index = 1;
		static const unsigned int normal_attribute_index = 2;

		virtual ~mesh() {}

		virtual void upload() = 0;
		virtual void render() const = 0;
	};
}

#endif
