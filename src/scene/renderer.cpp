//
// Created by Joseph Birkner on 5/8/16.
//

#include <stdexcept>
#include "renderer.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "shader/basic_program.hpp"
#include "basic_light.hpp"

using namespace indigo;

struct renderer::impl {
    impl() : current_cam(nullptr), current_scene(nullptr) {}
    camera const* current_cam;
    scene* current_scene;
};

renderer::renderer(float time) : time(time), impl_(new impl()) {
}

renderer::~renderer() {}

void renderer::render(camera const& cam, scene& scene) {
    if(impl_->current_cam || impl_->current_scene)
        throw std::runtime_error("attempt to make nested renderer::render(camera, scene) call!");

    glClearColor(1.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    impl_->current_cam = &cam;
    impl_->current_scene = &scene;

    render(scene.root(), glm::mat4());

    impl_->current_cam = nullptr;
    impl_->current_scene = nullptr;
}

void renderer::fetch(basic_shader_program& target)
{
    if(!impl_->current_cam || !impl_->current_scene)
        throw std::runtime_error("attempt to call renderer::fetch() without scene!");

    target.view(impl_->current_cam->view(time));
    target.projection(impl_->current_cam->projection());
    target.sun(impl_->current_scene->sun().color, impl_->current_scene->sun().position);
}

void renderer::render(entity& ent, glm::mat4 const& parent) {
    ent.update_world_matrix(parent, time);
    ent.render(*this);

    for(entity_shared_ptr& child : ent.children_)
        render(*child, ent.world_);
}
