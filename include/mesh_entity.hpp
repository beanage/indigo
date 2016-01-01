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

                enum render_mode {
                    filled,
                    wireframe
                };

                void rendermode(render_mode mode) {rendermode_ = mode;};
                render_mode rendermode() {return rendermode_;}

		virtual void render() const;

	private:
		const mesh* model_;
                render_mode rendermode_;
	};
}

#endif
