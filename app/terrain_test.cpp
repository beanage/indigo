#include <iostream>
#include <scene/mesh_entity.hpp>

#include "platform/window.hpp"
#include "app/application.hpp"
#include "app/keyboard_event.hpp"
#include "app/mouse_event.hpp"
#include "app/application_event.hpp"
#include "platform/resource_manager.hpp"
#include "model/mesh.hpp"
#include "scene/camera.hpp"

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

	void init() override
	{
		window_.title("Terrain Test");

		camera_.aspect_ratio(800.f/600.f);
		camera_.position({0.f, 0.f, 0.f});

		// auto& mesh_manager = indigo::resource_manager<mesh>::shared();
		// mesh_manager.add_path("media");
		// mesh_manager.add_loader(std::unique_ptr<obj_loader>(new obj_loader()));
		// orc_mesh = mesh_manager.load("monkey.obj");
		// if(!orc_mesh)
		// 	throw std::runtime_error("Failed to load orc!");
		// orc.attach_mesh(orc_mesh.get());

		add_event_handler(&key_handler_);
		add_event_handler(&cam_man_);
	}

	void update() override
	{
		cam_man_.move(camera_);
	}

	void render(float time) override
	{

	}

private:
	window window_;
	camera camera_;
	camera_man cam_man_;
	keyboard_handler key_handler_;
	std::shared_ptr<mesh> orc_mesh;
	mesh_entity orc;
};


int main(int argc, const char** argv)
{
	init_gl();

	terrain_test app;
	run(app, argc, argv);

	return 0;
}
