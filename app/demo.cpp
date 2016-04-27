#include "platform/window.hpp"
#include "model/mesh.hpp"
#include "model/model.hpp"
#include "model/obj/obj_loader.hpp"
#include "model/md5/md5_loader.hpp"
#include "shader/basic_program.hpp"
#include "shader/shader.hpp"
#include "scene/camera.hpp"
#include "scene/mesh_entity.hpp"
#include "util/log.hpp"
#include "texture.hpp"
#include "resource_manager.hpp"
#include "model/md5/md5_bone.hpp"
#include "terrain/terrain.hpp"
#include "bitmap.hpp"
#include "bitmap_loader.hpp"

#include <SDL2/SDL.h>
#include <unistd.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/intersect.hpp>
#include <memory>

#include "app/application.hpp"
#include "util/debug.hpp"

using namespace indigo;

class demo_application : public indigo::application
{
public:
    demo_application()
        : window_({0, 0, 800, 600}),
          program_md5_({indigo::load_shader("shader/md5-fragment-shader.shader", GL_FRAGMENT_SHADER), indigo::load_shader("shader/md5-vertex-shader.shader", GL_VERTEX_SHADER)}),
          program_obj_({indigo::load_shader("shader/default-fragment-shader.shader", GL_FRAGMENT_SHADER), indigo::load_shader("shader/default-vertex-shader.shader", GL_VERTEX_SHADER)}),
          program_cel_({indigo::load_shader("shader/cell-fragment-shader.frag", GL_FRAGMENT_SHADER), indigo::load_shader("shader/cell-vertex-shader.shader", GL_VERTEX_SHADER)}),
          mesh_(nullptr),
          entity_(nullptr),
          texture_("media/texture.png"),
          heightmap_("media/height.png"),
          grass_("media/grass.png"),
          earth_("media/sand.png"),
          rock_("media/snow.png"),
          materialtex_("media/materialmix.png"),
          key_w(false),
          key_s(false),
          key_a(false),
          key_d(false),
          inner_tess(32), // terrain tesselation
          tex_scale(8.f) // texture spans 8 tiles
    {
    }

    void init() override
    {
        // SDL_SetRelativeMouseMode(SDL_TRUE);

        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glClearDepth(1.0);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);

        int texture_units = 0, texture_objects = 0;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &texture_objects);
        std::cout << texture_units << " sim. texture units available" << std::endl;
        std::cout << texture_objects << " total texture objects available"
                  << std::endl;

        auto& mesh_manager = indigo::resource_manager<mesh>::shared();
        mesh_manager.add_path("media");
        mesh_manager.add_loader(std::unique_ptr<obj_loader>(new obj_loader()));

        auto& model_manager = indigo::resource_manager<model>::shared();
        model_manager.add_path("media");
        model_manager.add_loader(std::unique_ptr<md5_loader>(new md5_loader()));

        auto& bitmap_manager = indigo::resource_manager<bitmap>::shared();
        bitmap_manager.add_path("media");
        bitmap_manager.add_loader(
            std::unique_ptr<bitmap_loader>(new bitmap_loader()));

        model_ = model_manager.load("bob.md5mesh");
        model_->upload();

        mesh_ = mesh_manager.load("mesh.obj");
        mesh_->upload();

        camera_.aspect_ratio(800.f / 600.f);
        camera_.position({0.f, 0.f, 0.f});

        entity_.position({0.f, 0.f, 0.f});
        entity_.attach_mesh(mesh_.get());

        auto heightmap =
            indigo::resource_manager<bitmap>::shared().load("heightmap.png");
        auto opaquemap =
            indigo::resource_manager<bitmap>::shared().load("opaquemap.png");

        terrain_ = new terrain(8, 8);
        terrain_->heightmap(heightmap);
        terrain_->opacitymap(opaquemap);
        terrain_->upload();
    }

    void update() override
    {
        update_keys();

        if ((cube_accell_.x > .01f || cube_accell_.x < -.01f) ||
            (cube_accell_.y > .01f || cube_accell_.y < -.01f))
            cube_accell_ /= 5.01f;
        else
            cube_accell_ = glm::vec2(0.f, 0.f);

        int mx, my;
        Uint8 butt = SDL_GetRelativeMouseState(&mx, &my);

        bool lbt = butt & SDL_BUTTON(SDL_BUTTON_LEFT);
        bool rbt = butt & SDL_BUTTON(SDL_BUTTON_RIGHT);

        SDL_SetRelativeMouseMode(rbt || lbt ? SDL_TRUE : SDL_FALSE);

        if (cube_accell_ != glm::vec2(0.f, 0.f)) {
            entity_.turn_global(cube_accell_.x, cube_accell_up_);
            entity_.turn_global(cube_accell_.y, cube_accell_right_);
        }

        if (lbt) {
            cube_accell_up_ = glm::inverse(entity_.rotation()) * camera_.up();
            cube_accell_right_ = glm::inverse(entity_.rotation()) * camera_.right();
            cube_accell_ += glm::vec2(mx / 2.f, my / 2.f);
        } else if (rbt) {
            float pitch = my / 10.f;
            float yaw = mx / 10.f;

            camera_.turn(yaw, pitch);
        }

        glm::vec3 velocity(0, 0, 0);
        if (key_w)
            velocity += camera_.forward() * .1f;
        if (key_s)
            velocity += camera_.forward() * -.1f;
        if (key_a)
            velocity += camera_.right() * -.1f;
        if (key_d)
            velocity += camera_.right() * .1f;

        camera_.move(velocity);

        int abs_mx(0), abs_my(0);
        SDL_GetMouseState(&abs_mx, &abs_my);

        // Cursor-Ray
        glm::vec2 cursor =
            glm::vec2(abs_mx, abs_my) / glm::vec2(800, 600) * 2.f - 1.f;
        cursor.y = -cursor.y;

        glm::mat4 world_matrix =
            glm::inverse(camera_.projection() * camera_.view());

        glm::vec4 ray_from = world_matrix * glm::vec4(cursor.x, cursor.y, -1, 1);
        glm::vec4 ray_to = world_matrix * glm::vec4(cursor.x, cursor.y, 1, 1);

        ray_from /= ray_from.w;
        ray_to /= ray_to.w;

        glm::vec3 triangles[] = {{1.f / 2, -1.f / 2, 0},
                                 {1.f / 2, 1.f / 2, 0},
                                 {-1.f / 2, 1.f / 2, 0},
                                 {-1.f / 2, -1.f / 2, 0}};

        glm::vec3 direction =
            glm::normalize(glm::vec3(ray_to) - glm::vec3(ray_from));
        // direction.z = -direction.z;

        // std::cout << "Mouse Ray from: " << glm::vec3(ray_from) << " direction: "
        // << direction << std::endl;

        glm::vec3 intersection;
        bool intersect = false;
        // intersect |= glm::intersectRayTriangle(glm::vec3(ray_from), direction,
        // glm::vec3(glm::vec4(triangles[0], 1) * entity_.model()),
        // glm::vec3(glm::vec4(triangles[1], 1) * entity_.model()),
        // glm::vec3(glm::vec4(triangles[2], 1) * entity_.model()), intersection);
        // intersect |= glm::intersectRayTriangle(glm::vec3(ray_from), direction,
        // glm::vec3(glm::vec4(triangles[2], 1) * entity_.model()),
        // glm::vec3(glm::vec4(triangles[3], 1) * entity_.model()),
        // glm::vec3(glm::vec4(triangles[0], 1) * entity_.model()), intersection);

        unsigned tile_x, tile_y;
        // intersect = cell_->intersects(glm::vec3(ray_from), direction,
        // glm::inverse(entity_.model()), intersection, tile_x, tile_y);

        if (intersect) {
            glm::vec4 screen = glm::vec4(intersection, 1) * entity_.model() *
                               camera_.view() * camera_.projection();
            // program_cel_.set("cursor", glm::vec3(screen));

            std::cout << "Hovered Tile: " << tile_x << ", " << tile_y << std::endl;
        } else {
            // program_cel_.set("cursor", glm::vec3(0, 0, 0));
        }
    }

    void render_obj(float time)
    {
        program_obj_.use();
        program_obj_.set("projection", camera_.projection());
        program_obj_.set("view", camera_.view(time));
        program_obj_.set("model", entity_.model(time));

        program_obj_.set("light_1.position", camera_.position());
        program_obj_.set("light_1.color", glm::vec3(1, 1, 1));
        // program_obj_.set("light_1.attenuation", 0.2f);
        // program_obj_.set("light_1.ambient_coefficient", 0.05f);
        // program_obj_.set("material_1.specular_color", glm::vec3(1, 1, 1));
        // program_obj_.set("material_1.specular_exponent", 0.f);
        // program_obj_.set("camera_pos", camera_.position());

        texture_.bind();
        entity_.render();
    }

    void render_md5(float time)
    {
        program_md5_.use();
        program_md5_.set("projection", camera_.projection());
        program_md5_.set("view", camera_.view(time));
        program_md5_.set("model", entity_.model(time));

        std::vector<glm::mat4> bone_matrix;
        for (auto const& b : model_->bones())
            bone_matrix.push_back(static_cast<md5_bone*>(b)->model());

        program_md5_.set("bones", bone_matrix);
        program_md5_.set("weight_tex", (int)0);

        model_->render();
    }

    void render_terrain(float time)
    {
        program_cel_.use();
        program_cel_.set("projection", camera_.projection());
        program_cel_.set("view", camera_.view(time));
        program_cel_.set("model", entity_.model(time));

        glActiveTexture(GL_TEXTURE0);
        program_cel_.set("tex0", 0);
        grass_.bind();

        glActiveTexture(GL_TEXTURE1);
        program_cel_.set("tex1", 1);
        earth_.bind();

        glActiveTexture(GL_TEXTURE2);
        program_cel_.set("tex2", 2);
        rock_.bind();

        glActiveTexture(GL_TEXTURE3);
        program_cel_.set("blend", 3);
        materialtex_.bind();

        glDisable(GL_CULL_FACE);

        terrain_->render(program_cel_);
    }

    void render(float time) override
    {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render_obj(time);
        // render_md5(time);
        render_terrain(time);

        camera_.update();
        entity_.update();

        window_.swap();
    }

    void update_keys()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
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
                if (event.key.keysym.sym == SDLK_i)
                    inner_tess++;
                if (event.key.keysym.sym == SDLK_k)
                    inner_tess--;
                if (event.key.keysym.sym == SDLK_u)
                    tex_scale++;
                if (event.key.keysym.sym == SDLK_j)
                    tex_scale--;
                if (event.key.keysym.sym == SDLK_l)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                if (event.key.keysym.sym == SDLK_f)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
    indigo::program program_md5_;
    indigo::program program_obj_;
    indigo::program program_cel_;
    indigo::camera camera_;
    indigo::texture texture_;
    indigo::texture heightmap_;
    indigo::texture grass_;
    indigo::texture earth_;
    indigo::texture rock_;
    indigo::texture materialtex_;
    indigo::terrain* terrain_;

    std::shared_ptr<indigo::model> model_;
    std::shared_ptr<indigo::mesh> mesh_;
    indigo::mesh_entity entity_;

    bool key_w, key_s, key_a, key_d;

    float inner_tess, tex_scale;

    glm::vec2 cube_accell_;
    glm::vec3 cube_accell_up_;
    glm::vec3 cube_accell_right_;
};

int main(int argc, char const** argv)
{
    indigo::init_gl();

    demo_application app;
    indigo::run(app, argc, argv);

    return 0;
}
