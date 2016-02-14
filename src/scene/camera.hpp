#ifndef __CAMERA_HPP_INCLUDED__
#define __CAMERA_HPP_INCLUDED__

#include <glm/mat4x4.hpp>
#include "scene/entity.hpp"

namespace indigo
{
	class camera : public entity
	{
	public:
        enum class view_mode
        {
            perspective,
            ortho
        };

		camera();

        view_mode mode() const;
        void mode(view_mode m);

		float aspect_ratio() const;
		void aspect_ratio(float ratio);

		float fov() const;
		void fov(float fov);

		glm::mat4 projection() const;
        glm::mat4 view() const;
        glm::mat4 view(float step) const;

        void turn(float yaw, float pitch);

    protected:
        glm::mat4 build_model_martix(const glm::vec3& pos, const glm::quat& rot) const override;

	private:
		float fov_;
		float near_;
		float far_;
		float ratio_;
        view_mode mode_;
	};
}

#endif
