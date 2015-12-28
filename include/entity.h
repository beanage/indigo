#ifndef __ENTITY_H_INCLUDED__
#define __ENTITY_H_INCLUDED__

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace indigo
{
	class entity
	{
	public:
		virtual void render() {}

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

	protected:
		entity();

	private:
		glm::mat4 model_;
		glm::vec3 position_;
		glm::vec3 rotation_;
	};
}

#endif
