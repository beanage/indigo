#include <iostream>

#include "platform/window.hpp"
#include "app/application.hpp"
#include "app/keyboard_event.hpp"
#include "app/event_visitor.hpp"

#include "scene/camera.hpp"
#include "scene/entity.hpp"

using namespace indigo;

class keyboard_handler : public event_visitor
{
public:
	keyboard_handler(application& app)
		: application_(app)
	{}

	bool visit(const key_down_event& e) override
	{
		if (e.modifiers() & key_modifier::ctrl) {
			switch (e.keycode()) {
				case 'q': application_.terminate(); break;
				default: return false;
			}

			return true;
		}

		return false;
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
		switch (e.keycode()) {
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
		switch (e.keycode()) {
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

		add_event_handler(&key_handler_);
		add_event_handler(&cam_man_);
	}

	void update() override
	{
		cam_man_.move(camera_);
	}

	void render(float time) override
	{}

private:
	window window_;
	camera camera_;
	camera_man cam_man_;
	keyboard_handler key_handler_;
};


int main(int argc, const char** argv)
{
	init_gl();

	terrain_test app;
	run(app, argc, argv);

	return 0;
}
