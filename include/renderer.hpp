#ifndef __INDIGO_RENDERER_HPP__
#define __INDIGO_RENDERER_HPP__

#include "camera.hpp"
#include "scene.hpp"

namespace indigo
{
    class application;

    class renderer {
        friend class applicaton;

    private:
        renderer(float time) : time(time) {}

        void render(camera const& cam, scene& scene);

        float time;
    };
}

#endif
