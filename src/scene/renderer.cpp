//
// Created by Joseph Birkner on 5/8/16.
//

#include <stdexcept>
#include "renderer.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "shader/basic_program.hpp"

using namespace indigo;

struct renderer::impl {
    impl(float time) : time(time), current_cam(nullptr), current_scene(nullptr) {}
    float time;
    camera const* current_cam;
    scene* current_scene;
};

renderer::renderer(float time) : impl_(new impl(time)) {
}

void renderer::render(camera const& cam, scene& scene) {
    if(impl_->current_cam || impl_->current_scene)
        throw std::runtime_error("attempt to make nested renderer::render(camera, scene) call!");
    impl_->current_cam = &cam;
    impl_->current_scene = &scene;

    render(scene.root(), glm::mat4());

    impl_->current_cam = nullptr;
    impl_->current_scene = nullptr;
}

void renderer::fetch(basic_shader_program& target)
{
    target.view(impl_->current_cam->view());
    target.projection(impl_->current_cam->projection());
}

void renderer::render(entity& ent, glm::mat4 const& parent) {
    ent.update_world_matrix(parent, impl_->time);
    ent.render();

    for(entity_shared_ptr& child : ent.children_)
        render(*child, ent.world_);
}
