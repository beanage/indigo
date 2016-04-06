#include "app/keyboard_event.hpp"
#include "app/event_visitor.hpp"

using namespace indigo;

key_down_event::key_down_event(unsigned key, unsigned mods, unsigned repeat)
	: basic_key_event(key, mods)
	, repeat_count(repeat)
{}

key_up_event::key_up_event(unsigned key, unsigned mods)
	: basic_key_event(key, mods)
{}
