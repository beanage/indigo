#include "application.hpp"
#include <SDL2/SDL.h>

using namespace indigo;

const std::chrono::milliseconds application::step_time(10);

application::application()
    : quit_(false)
{}

application::~application()
{}

void application::run(int argc, char const** argv)
{
	using std::chrono::milliseconds;

    init();

    milliseconds prev_time(SDL_GetTicks());
    double update_lag(0.);

	while (!quit_) {
        milliseconds cur_time(SDL_GetTicks());
        milliseconds time_div(cur_time - prev_time);
		prev_time = cur_time;
        update_lag += time_div.count();

        while (update_lag >= step_time.count()) {
            update();
            update_lag -= step_time.count();
		}

        render(update_lag / step_time.count());
	}
}

void application::quit()
{
    quit_ = true;
}
