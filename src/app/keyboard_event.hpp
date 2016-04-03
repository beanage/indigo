#ifndef __INDIGO_KEYBOARD_EVENT_INCLUDED__
#define __INDIGO_KEYBOARD_EVENT_INCLUDED__

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

class basic_key_event : public basic_event
{
public:
	~basic_key_event() {}
	
	unsigned keycode() const;
	unsigned modifiers() const;

protected:
	basic_key_event(unsigned key, unsigned mods);

private:
	basic_key_event() = delete;

	unsigned key_;
	unsigned modifiers_;
};

class key_down_event : public basic_key_event
{
public:
	key_down_event(unsigned key, unsigned mods, unsigned repeat);

	unsigned repeat_count() const;

	bool accept(event_visitor&) const override;

private:
	unsigned repeat_count_;
};

class key_up_event : public basic_key_event
{
public:
	key_up_event(unsigned key, unsigned mods);

	bool accept(event_visitor&) const override;
};
}

#endif
