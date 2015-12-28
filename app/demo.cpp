#include "window.hpp"
#include "mesh.hpp"
#include "obj_loader.hpp"
#include "obj_mesh.hpp"
#include "program.hpp"
#include "shader.hpp"
#include "camera.hpp"

#include <SDL2/SDL.h>
#include <unistd.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

int main(int argc, char** argv)
{
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

	glm::mat4 model;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDisable(GL_CULL_FACE); //TODO: Testing
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

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

		if (up) {
			model = glm::rotate(model, glm::radians((float)mx), glm::vec3(0.f, 1.f, 0.f));
		} else if (dw) {
			cam.position(cam.position() + cam.forward() * (my * 0.1f));
		}

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		mesh->render();

		program.set("model", model);
		program.set("projection", cam.projection());
		program.set("view", cam.view());

		win.swap();
	}

	return 0;
}
