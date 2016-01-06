#include "application.hpp"
#include <SDL2/SDL.h>

using namespace indigo;

const unsigned int application::gl_major_version(3);
const unsigned int application::gl_minor_version(3);
const std::chrono::milliseconds application::step_time(10);

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

void indigo::run(application& app, int argc, const char** argv)
{
    using std::chrono::milliseconds;

    app.init();

    milliseconds prev_time(SDL_GetTicks());
    double update_lag(0.);

    while (!app.terminated()) {
        milliseconds cur_time(SDL_GetTicks());
        milliseconds time_div(cur_time - prev_time);
        prev_time = cur_time;
        update_lag += time_div.count();

        while (update_lag >= application::step_time.count()) {
            app.update();
            update_lag -= application::step_time.count();
        }

        app.render(update_lag / application::step_time.count());
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
