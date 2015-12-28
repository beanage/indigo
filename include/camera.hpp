#ifndef __CAMERA_HPP_INCLUDED__
#define __CAMERA_HPP_INCLUDED__

#include <glm/mat4x4.hpp>
#include "entity.h"

namespace indigo
{
	class camera : public entity
	{
	public:
		camera();

		float aspect_ratio() const;
		void aspect_ratio(float ratio);

		float fov() const;
		void fov(float fov);

		glm::mat4 projection() const;
		const glm::mat4& view() const;

	private:
		float fov_;
		float near_;
		float far_;
		float ratio_;
	};
}

#endif
