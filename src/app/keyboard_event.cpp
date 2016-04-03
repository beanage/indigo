#include "app/keyboard_event.hpp"
#include "app/event_visitor.hpp"

using namespace indigo;

basic_key_event::basic_key_event(unsigned key, unsigned mods)
	: key_(key)
	, modifiers_(mods)
{}

unsigned basic_key_event::keycode() const
{
	return key_;
}

unsigned basic_key_event::modifiers() const
{
	return modifiers_;
}

key_down_event::key_down_event(unsigned key, unsigned mods, unsigned repeat)
	: basic_key_event(key, mods)
	, repeat_count_(repeat)
{}

bool key_down_event::accept(event_visitor& v) const
{
	v.visit(*this);
}

key_up_event::key_up_event(unsigned key, unsigned mods)
	: basic_key_event(key, mods)
{}

bool key_up_event::accept(event_visitor& v) const
{
	v.visit(*this);
}
