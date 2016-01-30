#include "window.hpp"
#include "mesh.hpp"
#include "obj_loader.hpp"
#include "program.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "mesh_entity.hpp"
#include "log.hpp"
#include "texture.hpp"
#include "resource_manager.hpp"
#include "scene.hpp"
#include "renderer.hpp"

#include <SDL2/SDL.h>
#include <unistd.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/constants.hpp>

#include "application.hpp"
#include "debug.hpp"

using namespace indigo;

class demo_application : public indigo::application
{
public:
    demo_application()
        : window_({0, 0, 800, 600})
        , program_({indigo::load_shader("../shader/default-fragment-shader.shader", GL_FRAGMENT_SHADER), indigo::load_shader("../shader/default-vertex-shader.shader", GL_VERTEX_SHADER)})
        , mesh_(nullptr)
        , entity_(nullptr)
        , texture_("../media/texture.png")
        , scene_(indigo::aabb(100.0,100.0,100.0))
        , key_w(false)
        , key_s(false)
        , key_a(false)
        , key_d(false)
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
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);

        program_.use();

        indigo::resource_manager<mesh> mesh_manager;
        mesh_manager.add_search_directory("../media");
        mesh_manager.instantiate_loader<obj_loader>();

        mesh_ = mesh_manager.load("gun.obj");
        mesh_->upload();

        camera_.aspect_ratio(800.f/600.f);
        camera_.position({50.f, 50.f, 50.f});
        camera_.look_at(entity_.position());

        entity_.position({0.f, 0.f, 0.f});
        entity_.attach_mesh(mesh_.get());
    }

    void update() override
    {
        update_keys();

        entity_.update();
        camera_.update();

        if ((cube_accell_.x > .01f || cube_accell_.x < -.01f) || (cube_accell_.y > .01f || cube_accell_.y < -.01f ))
            cube_accell_ /= 1.01f;
        else
            cube_accell_ = glm::vec2(0.f, 0.f);

        int mx, my;
        Uint8 butt = SDL_GetRelativeMouseState(&mx, &my);

        bool lbt = butt & SDL_BUTTON(SDL_BUTTON_LEFT);

        if (cube_accell_ != glm::vec2(0.f, 0.f)) {
            entity_.turn_global(cube_accell_.x, cube_accell_up_);
            entity_.turn_global(cube_accell_.y, cube_accell_right_);
        }

        if (lbt) {
            cube_accell_up_ = glm::inverse(entity_.rotation()) * camera_.up();
            cube_accell_right_ = glm::inverse(entity_.rotation()) * camera_.right();
            cube_accell_ += glm::vec2(mx/10.f, my/10.f);
        } else {
            float pitch = my/10.f;
            float yaw = mx/10.f;

            camera_.turn(yaw, pitch);
        }

        glm::vec3 velocity(0, 0, 0);
        if(key_w)
            velocity += camera_.forward() * .1f;
        if(key_s)
            velocity += camera_.forward() * -.1f;
        if(key_a)
            velocity += camera_.right() * -.1f;
        if(key_d)
            velocity += camera_.right() * .1f;
#ifdef DEBUG
        camera_.move(velocity);
#endif
    }

    void render(renderer const& r) override
    {
        r.render(camera_, scene_);
        window_.swap();
    }

    void update_keys()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch(event.type) {
            case SDL_QUIT:
                terminate();
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_w)
                    key_w = true;
                if (event.key.keysym.sym == SDLK_s)
                    key_s = true;
                if (event.key.keysym.sym == SDLK_a)
                    key_a = true;
                if (event.key.keysym.sym == SDLK_d)
                    key_d = true;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_w)
                    key_w = false;
                if (event.key.keysym.sym == SDLK_s)
                    key_s = false;
                if (event.key.keysym.sym == SDLK_a)
                    key_a = false;
                if (event.key.keysym.sym == SDLK_d)
                    key_d = false;
                break;
            }
        }
    }

private:
    indigo::window window_;
    indigo::camera camera_;
    indigo::texture texture_;
    indigo::scene scene_;

    std::shared_ptr<indigo::mesh> mesh_;
    indigo::mesh_entity entity_;

    bool key_w, key_s, key_a, key_d;
    glm::vec2 cube_accell_;
    glm::vec3 cube_accell_up_;
    glm::vec3 cube_accell_right_;
};

int main(int argc, char const ** argv)
{
    indigo::init_gl();

    demo_application app;
    indigo::run(app, argc, argv);

    return 0;
}
