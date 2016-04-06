#pragma once

#include "basic_event.hpp"

namespace indigo
{
enum class mouse_button
{
	left  = 0, // left mouse button
	right = 1, // right mouse button
	mid   = 2, // middle mouse button
	alt_0 = 3,
	alt_1 = 4,
};

template <class SubClass>
class basic_mouse_event : public basic_event_t<SubClass>
{
public:
	float x;
	float y;
	unsigned device;

	virtual ~basic_mouse_event() {}

protected:
	basic_mouse_event(float x, float y, unsigned device)
		: x(x), y(y), device(device)
	{}

	basic_mouse_event() = delete;
};

class mouse_move_event : public basic_mouse_event<mouse_move_event>
{
public:
	mouse_move_event(float x, float y, unsigned device);
	mouse_move_event() = delete;
};

class mouse_down_event : public basic_mouse_event<mouse_down_event>
{
public:
	mouse_button button;
	bool double_click;

	mouse_down_event(float x, float y, unsigned device, mouse_button button, bool double_click);
	mouse_down_event() = delete;
};

class mouse_up_event : public basic_mouse_event<mouse_up_event>
{
public:
	mouse_button button;

	mouse_up_event(float x, float y, unsigned device, mouse_button button);
	mouse_up_event() = delete;
};

class mouse_wheel_event : public basic_mouse_event<mouse_wheel_event>
{
public:
	int delta;

	mouse_wheel_event(float x, float y, unsigned device, int wheel_delta);
	mouse_wheel_event() = delete;
};
}
