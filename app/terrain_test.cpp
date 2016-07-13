#include <iostream>
#include <scene/mesh_entity.hpp>

#include "platform/window.hpp"
#include "app/application.hpp"
#include "app/keyboard_event.hpp"
#include "app/mouse_event.hpp"
#include "app/application_event.hpp"
#include "platform/resource_manager.hpp"
#include "shader/default_program.hpp"

#include "model/mesh.hpp"
#include "scene/camera.hpp"
#include "scene/renderer.hpp"
#include "scene/scene.hpp"
#include "scene/global_light.hpp"

#include <iostream>
#include <SDL2/SDL.h>

using namespace indigo;

class keyboard_handler : public event_visitor
{
public:
    keyboard_handler(application& app)
        : application_(app)
    {}

    bool visit(const app_terminate_event& e) override
    {
        application_.terminate();
        return true;
    }
    
    bool visit(const key_down_event& e) override
    {
        if (e.modifiers & key_modifier::ctrl) {
            switch (e.keycode) {
                case 'q': application_.terminate(); break;
                default: return false;
            }

            return true;
        }

        switch (e.keycode) {
            case key_code::esc: application_.terminate(); break;
            default: return false;
        }

        return true;
    }

private:
    application& application_;
};

class camera_man : public event_visitor
{
public:
    camera_man()
        : forward_(false)
        , backward_(false)
        , left_(false)
        , right_(false)
        , speed_(.1f)
        , rotx_(0.f)
        , roty_(0.f)
    {}

    bool visit(const key_down_event& e) override
    {
        switch (e.keycode) {
            case 'w': forward_ = true; break;
            case 'a': left_ = true; break;
            case 's': backward_ = true; break;
            case 'd': right_ = true; break;
            default: return false;
        }

        return true;
    }

    bool visit(const key_up_event& e) override
    {
        switch (e.keycode) {
            case 'w': forward_ = false; break;
            case 'a': left_ = false; break;
            case 's': backward_ = false; break;
            case 'd': right_ = false; break;
            default: return false;
        }

        return true;
    }

    bool visit(const mouse_move_event& e) override
    {
            rotx_ = e.x_rel / 3.f;
            roty_ = e.y_rel / 3.f;

            return true;
    }

    void move(camera& cam)
    {
        glm::vec3 velocity;
        if (forward_)
            velocity += cam.forward() * speed_;
        if (backward_)
            velocity += cam.forward() * -speed_;
        if (left_)
            velocity += cam.right() * -speed_;
        if (right_)
            velocity += cam.right() * speed_;

        cam.move(velocity);
        cam.turn(rotx_, roty_);

        rotx_ = roty_ = 0.f;
    }

private:
    bool forward_;
    bool backward_;
    bool left_;
    bool right_;
    float speed_;
    float rotx_;
    float roty_;
};

class terrain_test : public application
{
public:
    terrain_test()
        : window_({0, 0, 800, 600})
        , key_handler_(*this)
        , scene_({100, 100, 100})
    {}

    void init() override
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);

        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glClearDepth(1.0);
        glEnable(GL_DEPTH_TEST);

        window_.title("Terrain Test");

        camera_.aspect_ratio(800.f/600.f);
        camera_.position({0.f, 0.f, 0.f});

        auto& mesh_manager = indigo::resource_manager<mesh>::shared();
        orc_mesh = mesh_manager.load("monkey.obj");
        if(!orc_mesh)
            throw std::runtime_error("Failed to load orc!");
        orc_mesh->upload();

        orc = std::make_shared<mesh_entity>();
        orc->attach_mesh(orc_mesh.get());

        scene_.add(orc);

        add_event_handler(&key_handler_);
        add_event_handler(&cam_man_);
    }

    void update() override
    {
        cam_man_.move(camera_);
        scene_.sun({camera_.position(), glm::vec3(.0, 1., .0)});
    }

    void render(renderer& r) override
    {
        r.render(camera_, scene_);
        window_.swap();
    }

private:
    window window_;
    camera camera_;
    camera_man cam_man_;
    keyboard_handler key_handler_;
    std::shared_ptr<mesh> orc_mesh;
    std::shared_ptr<mesh_entity> orc;
    scene scene_;
};


int main(int argc, const char** argv)
{
    run<terrain_test>(argc, argv);
    return 0;
}
