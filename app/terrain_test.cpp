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

#include <iostream>

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
        : speed_(.1f)
        , forward_(false)
        , left_(false)
        , backward_(false)
        , right_(false)
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
    }

private:
    bool forward_;
    bool backward_;
    bool left_;
    bool right_;
    float speed_;
};

class terrain_test : public application
{
public:
    terrain_test()
        : window_({0, 0, 800, 600})
        , key_handler_(*this)
    {}

    virtual void pathes(resource_manager<model>& mgr) override {mgr.add_path("media");}
    virtual void pathes(resource_manager<mesh>& mgr) override {mgr.add_path("media");}
    virtual void pathes(resource_manager<shader>& mgr) override {mgr.add_path("shader");}
    virtual void pathes(resource_manager<bitmap>& mgr) override {mgr.add_path("media");}

    void init() override
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glClearDepth(1.0);
        glEnable(GL_DEPTH_TEST);

        window_.title("Terrain Test");

        camera_.aspect_ratio(800.f/600.f);
        camera_.position({0.f, 0.f, 0.f});

        def_shader.reset(new default_program);

        auto& mesh_manager = indigo::resource_manager<mesh>::shared();
        orc_mesh = mesh_manager.load("monkey.obj");
        if(!orc_mesh)
            throw std::runtime_error("Failed to load orc!");
        orc.attach_mesh(orc_mesh.get());
        orc_mesh->upload();

        add_event_handler(&key_handler_);
        add_event_handler(&cam_man_);
    }

    void update() override
    {
        cam_man_.move(camera_);
    }

    void render(float time /*renderer r*/) override
    {
        glClearColor(1.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // r.render(camera_, orc);
        def_shader->use();
        def_shader->uniform(uniform_projection, camera_.projection());
        def_shader->uniform(uniform_view, camera_.view(time));
        def_shader->uniform(uniform_model, orc.model(time));
        def_shader->uniform(uniform_light_1_position, camera_.position());
        def_shader->uniform(uniform_light_1_color, glm::vec3(1, 1, 1));

        orc.render();

        window_.swap();
    }

private:
    window window_;
    camera camera_;
    camera_man cam_man_;
    keyboard_handler key_handler_;
    std::shared_ptr<mesh> orc_mesh;
    std::unique_ptr<default_program> def_shader;
    mesh_entity orc;
};


int main(int argc, const char** argv)
{
    run<terrain_test>(argc, argv);
    return 0;
}
