#include "renderer.hpp"


void indigo::renderer::render(const indigo::camera& cam, indigo::scene& scene)
{
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    program_.set("light_1.position", camera_.position());
    program_.set("light_1.color", glm::vec3(1, 1, 1));
    // program_.set("light_1.attenuation", 0.2f);
    // program_.set("light_1.ambient_coefficient", 0.05f);
    // program_.set("material_1.specular_color", glm::vec3(1, 1, 1));
    // program_.set("material_1.specular_exponent", 0.f);
    // program_.set("camera_pos", camera_.position());
    program_.set("projection", camera_.projection());
    program_.set("view", camera_.view(time));
    program_.set("tex", GL_TEXTURE0);

    program_.set("model", entity_.model(time));
    texture_.bind();
    entity_.render();
}
