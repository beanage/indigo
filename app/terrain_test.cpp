#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "platform/window.hpp"
#include "app/application.hpp"
#include "app/keyboard_event.hpp"
#include "app/mouse_event.hpp"
#include "app/application_event.hpp"
#include "app/event_visitor.hpp"

#include "util/log.hpp"

#include "scene/camera.hpp"
#include "scene/entity.hpp"

#include "shader/shader.hpp"
#include "shader/program.hpp"

#include "mesh/mesh.hpp"

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
		, backward_(false)
		, left_(false)
		, right_(false)
	{}

	bool visit(const key_down_event& e) override
	{
		switch (e.keycode) {
			case 'w': forward_ = true; break;
			case 'a': left_ = true; break;
			case 's': backward_ = true; break;
			case 'd': right_ = true; break;
			case 'r': rotate_ = true; break;
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
			case 'r': rotate_ = false; break;
			default: return false;
		}

		return true;
	}

	bool visit(const mouse_down_event& e) override
	{
		if (e.button == mouse_button::left)
			mdown_ = true;

		return true;
	}

	bool visit(const mouse_up_event& e) override
	{
		if (e.button == mouse_button::left)
			mdown_ = false;

		return true;
	}

	bool visit(const mouse_move_event& e) override
	{
		mdelta_ = glm::vec2(e.x_rel, e.y_rel) / 2.5f;

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
		if (velocity != glm::vec3())
			cam.move(velocity);

		cam.turn(rotate_ * 1.f, 0.f);

		cam.turn(mdelta_.x, mdelta_.y);
		mdelta_ = {0, 0};
	}

private:
	bool forward_;
	bool backward_;
	bool left_;
	bool right_;
	bool mdown_;
	bool rotate_;
	glm::vec2 mdelta_;
	float speed_;
};

class phong_shader : public basic_shader_program
{
public:
	phong_shader()
		: vertex_(shader::type::vertex)
		, fragment_(shader::type::fragment)
	{
		vertex_.source(
			"#version 330\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"layout (location = 0) in vec3 position;\n"
			"layout (location = 1) in vec3 normal;\n"
			"layout (location = 2) in vec3 color;\n"
			"layout (location = 3) in vec2 uv;\n"
			"void main() { gl_Position = vec4(position, 1) * model * view * projection; }"
		);
		if (!vertex_.compile())
			log::write("Vertex shader compile failed!");


		fragment_.source(
			"#version 330\n"
			"out vec4 color;\n"
			"void main() { color = vec4(1, 1, 1, 1); }"
		);
		if (!fragment_.compile())
			log::write("Fragment shader compile failed!");

		attach_shader(vertex_);
		attach_shader(fragment_);

		if (!link())
			log::write("Link failed!");

		model_location_ = uniform_location("model");
		view_location_ = uniform_location("view");
		projection_location_ = uniform_location("projection");
	}

	void model(const glm::mat4& mat)
	{
		uniform(model_location_, mat);
	}

	void view(const glm::mat4& mat)
	{
		uniform(view_location_, mat);
	}

	void projection(const glm::mat4& mat)
	{
		uniform(projection_location_, mat);
	}

private:
	shader vertex_;
	shader fragment_;

	GLint model_location_;
	GLint view_location_;
	GLint projection_location_;
};

#include <SDL2/SDL.h>

class terrain_test : public application
{
public:
	terrain_test()
		: window_({0, 0, 800, 600})
		, key_handler_(*this)
	{}

	void init() override
	{
		window_.title("Terrain Test");

		camera_.aspect_ratio(800.f/600.f);
		camera_.position({0.f, 0.f, 0.f});

		add_event_handler(&key_handler_);
		add_event_handler(&cam_man_);

		mesh_ = create_plane_z();
		mesh_.upload();
		
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_BLEND);
		glClearDepth(1.f);
		glEnable(GL_DEPTH_TEST);

		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	void update() override
	{
		cam_man_.move(camera_);
	}

	void render(float time) override
	{
		glClearColor(0, 0, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_.use();
		shader_.model(glm::mat4());
		shader_.view(camera_.view(time));
		shader_.projection(camera_.projection());

		camera_.update();

		mesh_.render(shader_);

		window_.swap();
	}

private:
	window window_;
	camera camera_;
	camera_man cam_man_;
	keyboard_handler key_handler_;
	phong_shader shader_;
	mesh mesh_;
	glm::mat4 model_;
};


int main(int argc, const char** argv)
{
	init_gl();

	terrain_test app;
	run(app, argc, argv);

	return 0;
}
