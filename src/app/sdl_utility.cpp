#include "sdl_utility.hpp"
#include "keyboard_event.hpp"

using namespace indigo;

namespace indigo
{
namespace sdl_utility
{
unsigned translate_sdl_keycode(SDL_Keycode c)
{
	return static_cast<unsigned>(c);
}

unsigned translate_sdl_modifiers(Uint16 m)
{
	return (m & KMOD_LSHIFT ? key_modifier::lshift : 0) |
	       (m & KMOD_RSHIFT ? key_modifier::rshift : 0) |
	       (m & KMOD_LALT   ? key_modifier::lalt   : 0) |
	       (m & KMOD_RALT   ? key_modifier::ralt   : 0) |
	       (m & KMOD_LCTRL  ? key_modifier::lctrl  : 0) |
	       (m & KMOD_RCTRL  ? key_modifier::rctrl  : 0) |
	       (m & KMOD_LGUI   ? key_modifier::lsuper : 0) |
	       (m & KMOD_RGUI   ? key_modifier::rsuper : 0) |
	       (m & KMOD_MODE   ? key_modifier::mode   : 0) |
	       (m & KMOD_CAPS   ? key_modifier::caps   : 0) |
	       (m & KMOD_NUM    ? key_modifier::num    : 0);
}
}
}
