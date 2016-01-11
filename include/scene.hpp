#ifndef __INDIGO_SCENE_HPP__
#define __INDIGO_SCENE_HPP__

#include "basic_geom.hpp"
#include "entity.h"

#include <memory>

namespace indigo
{
    class scene
    {
    public:
        scene(aabb const& dimensions);

        void add(std::shared_ptr<scene> const& entity);

        std::shared_ptr<entity> pick_entity(ray const& here);

    private:
        struct impl;
        std::unique_ptr<impl> _impl;
    };
}

#endif
