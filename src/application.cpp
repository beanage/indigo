#include "application.hpp"
#include <SDL2/SDL.h>
#include <iostream>

using namespace indigo;

const unsigned int application::gl_major_version(3);
const unsigned int application::gl_minor_version(3);
const std::chrono::milliseconds application::update_intervall(10);

application::application()
    : quit_(false)
{}

application::~application()
{}

void application::terminate()
{
    quit_ = true;
}

bool application::terminated() const
{
    return quit_;
}

static void poll_events()
{
    //SDL_Event event;
    //while (SDL_PollEvent(&event) != 0) {
    //    switch (event.type) {
    //    default:;
        // TODO: Create event factories to convert SDL events to indigo::event subclasses
    //    }
    //}
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
            std::cout << app.ups_ << " - " << app.fps_ << std::endl;
        }

        poll_events();

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, application::gl_major_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, application::gl_minor_version);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetSwapInterval(1);
}
