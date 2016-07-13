#include "mesh_entity.hpp"
#include "model/mesh.hpp"
#include "platform/gl.hpp"
#include "shader/default_program.hpp"
#include "scene/renderer.hpp"

using namespace indigo;

std::unique_ptr<default_program> mesh_entity::def_shader;

mesh_entity::mesh_entity(const mesh* m)
    : model_(m)
{
    if (!def_shader)
        def_shader.reset(new default_program);
}

const mesh* mesh_entity::attached_mesh() const
{
    return model_;
}

void mesh_entity::attach_mesh(const mesh* m)
{
    model_ = m;
}

void mesh_entity::render(renderer &r) const
{
    if (model_) {
        def_shader->use();
        def_shader->model(world_);
        r.fetch(*def_shader);
        glPolygonMode(GL_FRONT_AND_BACK, rendermode_ == wireframe ? GL_LINE : GL_FILL);
        model_->render();
    }
}
