#include "application.hpp"
#include "sdl_utility.hpp"
#include "keyboard_event.hpp"
#include "application_event.hpp"
#include <iostream>

#include "shader/shader_loader.hpp"
#include "model/md5/md5_loader.hpp"
#include "model/obj/obj_loader.hpp"
#include "platform/bitmap_loader.hpp"

using namespace indigo;

const unsigned int application::gl_major_version(3);
const unsigned int application::gl_minor_version(3);
const std::chrono::milliseconds application::update_intervall(10);

application::application()
    : quit_(false)
{
}

application::~application()
{}

void application::event(const basic_event& e)
{
	for (auto v : event_handler_)
		if (e.accept(*v))
			break;
}

void application::terminate()
{
    quit_ = true;
}

bool application::terminated() const
{
    return quit_;
}

void application::add_event_handler(event_visitor* handler)
{
	if (!handler)
		throw std::runtime_error("Inserting NULL event handler!");
	event_handler_.push_back(handler);
}

void application::remove_event_handler(event_visitor* handler)
{
	auto iter = std::find(event_handler_.begin(), event_handler_.end(), handler);
	if (iter != event_handler_.end())
		event_handler_.erase(iter);
}

static void submit_app_terminate(SDL_Event* e, application& app)
{
	app.event(app_terminate_event());
}

static void submit_key_down(SDL_Event* e, application& app)
{
	app.event(key_down_event(
		sdl_utility::translate_sdl_keycode(e->key.keysym.sym),
		sdl_utility::translate_sdl_modifiers(e->key.keysym.mod),
		e->key.repeat
	));
}

static void submit_key_up(SDL_Event* e, application& app)
{
	app.event(key_up_event(
		sdl_utility::translate_sdl_keycode(e->key.keysym.sym),
		sdl_utility::translate_sdl_modifiers(e->key.keysym.mod)
	));
}

static void submit_mouse_move(SDL_Event* e, application& app)
{
	app.event(mouse_move_event(
		e->motion.x,
		e->motion.y,
		e->motion.xrel,
		e->motion.yrel,
		e->motion.which
	));
}

static void submit_mouse_down(SDL_Event* e, application& app)
{
	app.event(mouse_down_event(
		e->button.x,
		e->button.y,
		e->button.which,
		sdl_utility::translate_sdl_mousebutton(e->button.button),
		e->button.clicks > 1
	));
}

static void submit_mouse_up(SDL_Event* e, application& app)
{
	app.event(mouse_up_event(
		e->button.x,
		e->button.y,
		e->button.which,
		sdl_utility::translate_sdl_mousebutton(e->button.button)
	));
}

static void submit_mouse_wheel(SDL_Event* e, application& app)
{
	app.event(mouse_wheel_event(
		e->wheel.x,
		e->wheel.y,
		e->wheel.which,
		e->wheel.x + e->wheel.y
	));
}

void application::poll_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_QUIT:
				submit_app_terminate(&event, *this);
				break;
			case SDL_KEYUP:
				submit_key_up(&event, *this);
				break;
			case SDL_KEYDOWN:
				submit_key_down(&event, *this);
				break;
			case SDL_MOUSEMOTION:
				submit_mouse_move(&event, *this);
				break;
			case SDL_MOUSEBUTTONDOWN:
				submit_mouse_down(&event, *this);
				break;
			case SDL_MOUSEBUTTONUP:
				submit_mouse_up(&event, *this);
				break;
			case SDL_MOUSEWHEEL:
				submit_mouse_wheel(&event, *this);
				break;
			case SDL_TEXTEDITING:
			case SDL_TEXTINPUT:
				// Ignore this events
				break;
			default:
				std::cout << "Unhandled event " << event.type << std::endl;
		}
	}
}

void indigo::run(application& app, int argc, const char** argv)
{
    using std::chrono::milliseconds;

    init_gl();
    app.setup_resource_managers();
    app.init();

    milliseconds prev_time(SDL_GetTicks());
    double update_lag(0.);

    milliseconds prev_sec(SDL_GetTicks());
    double frames(0.), updates(0.);

    while (!app.terminated()) {
        milliseconds cur_time(SDL_GetTicks());
        milliseconds time_div(cur_time - prev_time);
        prev_time = cur_time;
        update_lag += time_div.count();

        if (cur_time - prev_sec > milliseconds(1000)) {
            float const time_div = static_cast<float>((cur_time - prev_sec).count());

            app.fps_ = frames / time_div * 1000.f;
            app.ups_ = updates / time_div * 1000.f;
            prev_sec = cur_time;
            frames = 0;
            updates = 0;
        }

        app.poll_events();

        for (unsigned i = 0; i < 100 && update_lag >= application::update_intervall.count(); ++i) {
            app.update();
            ++updates;
            update_lag -= application::update_intervall.count();
        }

        app.render(update_lag / application::update_intervall.count());
        ++frames;
    }
}

void application::setup_resource_managers()
{
	auto& model_manager = indigo::resource_manager<model>::shared();
	model_manager.add_loader(std::unique_ptr<md5_loader>(new md5_loader()));
	pathes(model_manager);

    auto& mesh_manager = indigo::resource_manager<mesh>::shared();
    mesh_manager.add_loader(std::unique_ptr<obj_loader>(new obj_loader()));
    pathes(mesh_manager);

	auto& bitmap_manager = indigo::resource_manager<bitmap>::shared();
	bitmap_manager.add_loader(std::unique_ptr<bitmap_loader>(new bitmap_loader()));
	pathes(bitmap_manager);

	auto& shader_manager = indigo::resource_manager<shader>::shared();
	shader_manager.add_loader(std::unique_ptr<shader_loader>(new shader_loader()));
	pathes(shader_manager);
}


void indigo::init_gl()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, application::gl_major_version);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, application::gl_minor_version);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetSwapInterval(1);
}
