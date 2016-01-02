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
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetSwapInterval(1);

    indigo::window win({0, 0, 800, 600});
    win.title("DEMO");

    indigo::program program({
        indigo::load_shader("../shader/default-fragment-shader.shader", GL_FRAGMENT_SHADER),
        indigo::load_shader("../shader/default-vertex-shader.shader", GL_VERTEX_SHADER)
    });
    program.use();

    indigo::obj_loader loader;
    std::unique_ptr<indigo::mesh> mesh(loader.load("../media/mesh.obj"));
    mesh->upload();

    indigo::mesh_entity ent1(mesh.get());
    ent1.position({2.f, 0.f, 0.f});

    indigo::mesh_entity ent2(mesh.get());
    ent2.position({-2.f, 0.f, 0.f});

    indigo::camera cam;
    cam.aspect_ratio(800.f/600.f);
    cam.position({10.f, 0.f, 10.f});
    cam.look_at(ent1.position());

    indigo::texture tex1("../media/texture.png");

    indigo::texture tex2(800, 600);
    indigo::framebuffer fbo(tex2);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);

    bool key_w(false), key_s(false), key_a(false), key_d(false);
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch(event.type) {
            case SDL_QUIT:
                return 0;
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
                if(event.key.keysym.sym == SDLK_r)
                    cam.look_at(ent1.position());
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

        int mx, my;
        Uint8 butt = SDL_GetRelativeMouseState(&mx, &my);

        bool lbt = butt & SDL_BUTTON(SDL_BUTTON_LEFT);

        if (lbt) {
            ent1.turn(mx, glm::inverse(ent1.rotation())*cam.up()).turn(my, glm::inverse(ent1.rotation())*cam.right());
        } else {
            cam.turn((float)mx/10.f, cam.up()).turn((float)my/10.f, cam.right());
        }

        glm::vec3 velocity(0,0,0);
        if(key_w)
            velocity += cam.forward() * .1f;
        if(key_s)
            velocity += cam.forward() * -.1f;
        if(key_a)
            velocity += cam.right() * -.1f;
        if(key_d)
            velocity += cam.right() * .1f;
        cam.move(velocity);

        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        if (lbt) {
                fbo.bind();

                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClearDepth(1.0f);
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        }

        //ent.look_at(cam.position());
        program.set("model", ent1.model());
        tex1.bind();
        ent1.render();
        fbo.unbind();

        program.set("model", ent2.model());
        tex2.bind();
        ent2.render();

        program.set("projection", cam.projection());
        program.set("view", cam.view());
        program.set("tex", GL_TEXTURE0);

        win.swap();
    }

    return 0;
}
