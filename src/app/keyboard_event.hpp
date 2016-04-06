#pragma once

#include "basic_event.hpp"

namespace indigo
{
struct key_modifier
{
enum
{
	lshift = 1,    // left shift
	rshift = 2,    // right shift
	lctrl  = 4,    // left ctrl
	rctrl  = 8,    // right ctrl
	lalt   = 16,   // left alt
	ralt   = 32,   // right alt
	lsuper = 64,   // left windows
	rsuper = 128,  // right windows
	mode   = 256,  // alt gr
	caps   = 512,  // capslock
	num    = 1024, // numlock

	shift  = (lshift|rshift),
	ctrl   = (lctrl|rctrl),
	alt    = (lalt|ralt),
	super  = (lsuper|rsuper),
};
};

struct key_code
{
enum
{
	backspace = 8,
	tab       = 9,
	ret       = 12,
	space     = 32,
	esc       = 27,
	del       = 127,
	capslock  = 1000,
	f1,
	f2,
	f3,
	f4,
	f5,
	f6,
	f7,
	f8,
	f9,
	f10,
	f11,
	f12,
	print,
	scrolllock,
	pause,
	insert,
	home,
	pageup,
	end,
	pagedown,
	right,
	left,
	down,
	up,
};
};

template <class SubClass>
class basic_key_event : public basic_event_t<SubClass>
{
public:
	unsigned keycode;
	unsigned modifiers;

	virtual ~basic_key_event() {}

protected:
	basic_key_event(unsigned key, unsigned mods)
		: keycode(key), modifiers(mods)
	{}

	basic_key_event() = delete;
};

class key_down_event : public basic_key_event<key_down_event>
{
public:
	unsigned repeat_count;

	key_down_event(unsigned key, unsigned mods, unsigned repeat);
	key_down_event() = delete;
};

class key_up_event : public basic_key_event<key_up_event>
{
public:
	key_up_event(unsigned key, unsigned mods);
	key_up_event() = delete;
};
}
