#include "mouse_event.hpp"

using namespace indigo;

mouse_move_event::mouse_move_event(float x, float y, unsigned device)
	: basic_mouse_event(x, y, device)
{}

mouse_down_event::mouse_down_event(float x, float y, unsigned device, mouse_button button, bool double_click)
	: basic_mouse_event(x, y, device)
	, button(button)
	, double_click(double_click)
{}

mouse_up_event::mouse_up_event(float x, float y, unsigned device, mouse_button button)
	: basic_mouse_event(x, y, device)
	, button(button)
{}

mouse_wheel_event::mouse_wheel_event(float x, float y, unsigned device, int delta)
	: basic_mouse_event(x, y, device)
	, delta(delta)
{}

