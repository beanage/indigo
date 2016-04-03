#include "application.hpp"
#include "sdl_utility.hpp"
#include "keyboard_event.hpp"
#include "application_event.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>

using namespace indigo;

const unsigned int application::gl_major_version(3);
const unsigned int application::gl_minor_version(3);
const std::chrono::milliseconds application::update_intervall(10);

application::application()
    : quit_(false)
{}

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

void application::poll_events()
{
	static const std::vector<std::pair<int, void (*)(SDL_Event*, application&)>> sdl_event_handlers = {
		std::make_pair(SDL_QUIT, &submit_app_terminate),
		std::make_pair(SDL_KEYDOWN, &submit_key_down),
		std::make_pair(SDL_KEYUP, &submit_key_up),
	};

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		auto func_iter = std::find_if(sdl_event_handlers.begin(), sdl_event_handlers.end(), [&](const decltype(sdl_event_handlers)::value_type& pair) {
			return pair.first == event.type;
		});

		if (func_iter != sdl_event_handlers.end())
			((*func_iter).second)(&event, *this);
		else
			std::cout << "Unhandled event " << event.type << std::endl;
	}
}

void indigo::run(application& app, int argc, const char** argv)
{
    using std::chrono::milliseconds;

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

        while (update_lag >= application::update_intervall.count()) {
            app.update();
            ++updates;
            update_lag -= application::update_intervall.count();
        }

        app.render(update_lag / application::update_intervall.count());
        ++frames;
    }
}

void indigo::init_gl()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                        application::gl_major_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                        application::gl_minor_version);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetSwapInterval(1);
}
