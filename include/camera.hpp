#ifndef __CAMERA_HPP_INCLUDED__
#define __CAMERA_HPP_INCLUDED__

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace indigo
{
	class camera
	{
	public:
		camera();

		glm::vec3 position() const;
		camera& position(const glm::vec3& pos);

		float aspect_ratio() const;
		camera& aspect_ratio(float ratio);

		float fov() const;
		camera& fov(float fov);

		float vertical() const;
		camera& vertical(float vert);

		float horizontal() const;
		camera& horizontal(float horz);

		glm::mat4 orientation() const;
		glm::mat4 projection() const;
		glm::mat4 view() const;

		glm::vec3 forward() const;
		glm::vec3 up() const;
		glm::vec3 right() const;

		camera& look_at(const glm::vec3& target);

	private:
		glm::vec3 position_;
		float horizontal_;
		float vertical_;
		float fov_;
		float near_;
		float far_;
		float ratio_;
	};
}

#endif
