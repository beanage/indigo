#include <stdexcept>

#include "renderer.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "shader/basic_program.hpp"
#include "global_light.hpp"

using namespace indigo;

struct renderer::render_state
{
    render_state()
        : current_cam(nullptr)
        , current_scene(nullptr)
        , current_entity(nullptr)
    {}

    camera const* current_cam;
    scene* current_scene;
    entity* current_entity;
};

renderer::renderer(float time)
    : time(time)
    , state_(nullptr)
{}

renderer::~renderer()
{}

void renderer::render(camera const& cam, scene& scene)
{
    if (state_)
        throw std::runtime_error("attempt to make nested renderer::render(camera, scene) call!");

    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_state state;
    state.current_scene = &scene;
    state.current_cam = &cam;
    state.current_entity = nullptr;

    state_ = &state;
    render(scene.root_, glm::mat4());
    state_ = nullptr;
}

void renderer::prepare(basic_shader_program& target)
{
    if (!state_)
        throw std::runtime_error("attempt to call renderer::prepare() while not rendering!");

    target.fetch(*this);
}

const scene& renderer::get_scene() const
{
    if (!state_ || !state_->current_scene)
        throw std::runtime_error("attempt to call renderer::get_scene() without scene!");

    return *state_->current_scene;
}

const camera& renderer::get_camera() const
{
    if (!state_ || !state_->current_cam)
        throw std::runtime_error("attempt to call renderer::get_camera() without camera!");

    return *state_->current_cam;
}

const entity& renderer::get_entity() const
{
    if (!state_ || !state_->current_entity)
        throw std::runtime_error("attempt to call renderer::get_entity() without entity!");

    return *state_->current_entity;
}

void renderer::render(entity& ent, glm::mat4 const& parent)
{
    state_->current_entity = &ent;

    ent.update_world_matrix(parent, time);
    ent.render(*this);

    for (entity_shared_ptr& child : ent.children_)
        render(*child, ent.world_);
}
