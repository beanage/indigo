#ifndef __MESH_ENTITY_HPP_INCLUDED__
#define __MESH_ENTITY_HPP_INCLUDED__

#include "entity.h"

namespace indigo
{
	class mesh;

	class mesh_entity : public entity
	{
	public:
		mesh_entity(const mesh* m);

		//const mesh* model() const;
		//void model(const mesh* m);

		virtual void render() const;

	private:
		const mesh* model_;
	};
}

#endif
