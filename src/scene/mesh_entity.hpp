#pragma once

#include "scene/entity.hpp"

namespace indigo {
class mesh;

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

    virtual void render() const;

private:
    const mesh* model_;
    render_mode rendermode_;
};
}
