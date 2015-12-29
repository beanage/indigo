#include "window.hpp"
#include "mesh.hpp"
#include "obj_loader.hpp"
#include "obj_mesh.hpp"
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

	indigo::camera cam;
	cam.aspect_ratio(800.f/600.f);
	cam.position({0.f, 1.f, 10.f});

	indigo::mesh_entity ent(mesh.get());

	indigo::texture tex("../media/texture.png");
	tex.bind();

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);

	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT)
				return 0;
		}

		int mx, my;
		Uint8 butt = SDL_GetRelativeMouseState(&mx, &my);

		bool up = butt & SDL_BUTTON(SDL_BUTTON_LEFT);
		bool dw = butt & SDL_BUTTON(SDL_BUTTON_RIGHT);
		bool mb = butt & SDL_BUTTON(SDL_BUTTON_MIDDLE);

		if (up) {
			ent.rotation(ent.rotation() + glm::vec3((float)my, (float)mx, 0.f));
		} else if (dw) {
			cam.position(cam.position() + cam.forward() * (my * 0.1f));
		} else if (mb) {
			cam.rotation(cam.rotation() + glm::vec3((float)my, (float)mx, 0.f));
		}

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		ent.render();

		program.set("model", ent.model());
		program.set("projection", cam.projection());
		program.set("view", cam.view());
		program.set("tex", GL_TEXTURE0);

		win.swap();
	}

	return 0;
}
