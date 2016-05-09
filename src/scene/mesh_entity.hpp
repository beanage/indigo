#pragma once

#include "scene/entity.hpp"

namespace indigo {
class mesh;
class default_program;

class mesh_entity : public entity
{
public:
    mesh_entity(const mesh* m= nullptr);

    const mesh* attached_mesh() const;
    void attach_mesh(const mesh* m);

    enum render_mode
    {
        filled,
        wireframe
    };

    void rendermode(render_mode mode)
    {
        rendermode_ = mode;
    }
    render_mode rendermode()
    {
        return rendermode_;
    }

    virtual void render(renderer &r) const;

private:
    static std::unique_ptr<default_program> def_shader;

    const mesh* model_;
    render_mode rendermode_;

};
}
