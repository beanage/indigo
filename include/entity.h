#ifndef __ENTITY_H_INCLUDED__
#define __ENTITY_H_INCLUDED__

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "basic_geom.hpp"

#include <tuple>

namespace indigo
{
	class entity
	{
	public:
                virtual ~entity() {}

		virtual void render() const {}

		const glm::vec3& position() const;
		void position(const glm::vec3& pos);

		const glm::vec3& rotation() const;
		void rotation(const glm::vec3& rot);

		glm::mat4 orientation() const;
		const glm::mat4& model() const;

		glm::vec3 forward() const;
		glm::vec3 up() const;
		glm::vec3 right() const;

		void look_at(const glm::vec3& target);

                aabb axis_aligned_bounding_box(glm::mat4 const& abs_transform);

                virtual box bounding_box();
                virtual std::pair<bool, double> intersect(ray const& r);

	protected:
                entity() = default;
                entity(const entity&) = default;

	private:
                glm::mat4 model_;
		glm::vec3 position_;
		glm::vec3 rotation_;
	};
}

#endif
