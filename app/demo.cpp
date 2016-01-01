#include "window.hpp"
#include "mesh.hpp"
#include "obj_loader.hpp"
#include "program.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "mesh_entity.hpp"
#include "log.hpp"
#include "texture.hpp"

#include <SDL2/SDL.h>
#include <unistd.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include "debug.hpp"

using namespace indigo;

int main(int argc, char** argv)
{
    indigo::log("starting demo...");

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetSwapInterval(1);

    indigo::window win({0, 0, 800, 600});
    win.title("DEMO");

    indigo::program program({
        indigo::load_shader("../shader/default-fragment-shader.shader", GL_FRAGMENT_SHADER),
        indigo::load_shader("../shader/default-vertex-shader.shader", GL_VERTEX_SHADER)});
    program.use();

    indigo::obj_loader loader;
    std::unique_ptr<indigo::mesh> mesh(loader.load("../media/mesh.obj"));
    mesh->upload();

    indigo::mesh_entity ent(mesh.get());
    ent.position({2.f, 0.f, 0.f});
    ent.rendermode(indigo::mesh_entity::wireframe);

    indigo::camera cam;
    cam.aspect_ratio(800.f/600.f);
    cam.position({250.f, 0.f, 250.f});
    cam.look_at(ent.position());

    //indigo::texture tex("../media/texture.png");
    indigo::texture tex(indigo::colors::red, 512, 512);
    tex.bind();

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);

    bool key_w(false), key_s(false);

    bool lctrl_pressed = false;
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch(event.type) {
            case SDL_QUIT:
                return 0;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_LCTRL)
                    lctrl_pressed = true;
                if (event.key.keysym.sym == SDLK_w)
                    key_w = true;
                if (event.key.keysym.sym == SDLK_s)
                    key_s = true;
                break;
            case SDL_KEYUP:
                if(event.key.keysym.sym == SDLK_LCTRL)
                    lctrl_pressed = false;
                if(event.key.keysym.sym == SDLK_r)
                    cam.look_at(ent.position());
                if (event.key.keysym.sym == SDLK_w)
                    key_w = false;
                if (event.key.keysym.sym == SDLK_s)
                    key_s = false;
                break;
            }
        }

        ent.turn(.1, ent.up());
        ent.turn(.11, ent.right());

        int mx, my;
        Uint8 butt = SDL_GetRelativeMouseState(&mx, &my);

        bool lbt = butt & SDL_BUTTON(SDL_BUTTON_LEFT);
        bool rbt = butt & SDL_BUTTON(SDL_BUTTON_RIGHT);
        bool mbt = butt & SDL_BUTTON(SDL_BUTTON_MIDDLE);

        if (lbt) {
            glm::quat rotation = ent.rotation();
            rotation *= glm::angleAxis(glm::radians((float)mx), glm::vec3(0.f, 1.f, 0.f));
            rotation *= glm::angleAxis(glm::radians((float)my), glm::vec3(1.f, 0.f, 0.f));

            ent.rotation(rotation);
        } else if (rbt && !lctrl_pressed) {
            cam.position(cam.position() + cam.forward() * (my * 0.1f));
        } else if (mbt || lctrl_pressed) {
            std::cout << cam.right() << cam.forward() << std::endl;
            cam.turn((float)mx/10.f, glm::vec3(0.f, 1.f, 0.f));
        }

        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //ent.look_at(cam.position());
        ent.render();

        program.set("model", ent.model());
        program.set("projection", cam.projection());
        program.set("view", cam.view());
        program.set("tex", GL_TEXTURE0);

        win.swap();
    }

    return 0;
}
