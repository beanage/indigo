//
// Created by Joseph Birkner on 5/7/16.
//

#ifndef __INDIGO_SCENE_HPP__
#define __INDIGO_SCENE_HPP__

#include <memory>
#include "vec3.hpp"

namespace indigo
{
    class entity;
    class renderer;
    typedef std::shared_ptr<entity> entity_shared_ptr;

    class scene
    {
        friend class renderer;

        scene(glm::vec3 const& size);
        scene(scene const&) = delete;

        scene& add(entity_shared_ptr& ent);
        scene& remove(entity_shared_ptr const& ent);

    private:
        entity& root();

        struct impl;
        std::unique_ptr<impl> impl_;
    };
}


#endif //__INDIGO_SCENE_HPP__
