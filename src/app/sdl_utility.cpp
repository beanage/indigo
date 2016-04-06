#include "sdl_utility.hpp"
#include "keyboard_event.hpp"

using namespace indigo;

namespace indigo
{
namespace sdl_utility
{
#define KEY_TABLE(c)                                \
	c(SDLK_BACKSPACE,  backspace,  "backspace") \
	c(SDLK_TAB,        tab,        "tab")       \
	c(SDLK_RETURN,     ret,        "return")    \
	c(SDLK_SPACE,      space,      "space")     \
	c(SDLK_ESCAPE,     esc,        "escape")    \
	c(SDLK_DELETE,     del,        "delete")    \
	c(SDLK_CAPSLOCK,   capslock,   "capslock")  \
	c(SDLK_F1,         f1,         "f1")        \
	c(SDLK_F2,         f2,         "f2")        \
	c(SDLK_F3,         f3,         "f3")        \
	c(SDLK_F4,         f4,         "f4")        \
	c(SDLK_F5,         f5,         "f5")        \
	c(SDLK_F6,         f6,         "f6")        \
	c(SDLK_F7,         f7,         "f7")        \
	c(SDLK_F8,         f8,         "f8")        \
	c(SDLK_F9,         f9,         "f9")        \
	c(SDLK_F10,        f10,        "f10")       \
	c(SDLK_F11,        f11,        "f11")       \
	c(SDLK_F12,        f12,        "f12")       \
	c(SDLK_PRINTSCREEN,print,      "print")     \
	c(SDLK_SCROLLLOCK, scrolllock, "scrolllock")\
	c(SDLK_PAUSE,      pause,      "pause")     \
	c(SDLK_INSERT,     insert,     "insert")    \
	c(SDLK_HOME,       home,       "home")      \
	c(SDLK_PAGEUP,     pageup,     "pageup")    \
	c(SDLK_END,        end,        "end")       \
	c(SDLK_PAGEDOWN,   pagedown,   "pagedown")  \
	c(SDLK_RIGHT,      right,      "right")     \
	c(SDLK_LEFT,       left,       "left")      \
	c(SDLK_DOWN,       down,       "down")      \
	c(SDLK_UP,         up,         "up")

#define GEN_MAPPING(sdl, ind, unused) \
	case sdl: return key_code :: ind;

unsigned translate_sdl_keycode(SDL_Keycode c)
{
	switch (c) {
		KEY_TABLE(GEN_MAPPING)
		default:
			return static_cast<unsigned>(c);
	}
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

mouse_button translate_sdl_mousebutton(Uint8 button)
{
	return (button == SDL_BUTTON_LEFT)   ? mouse_button::left  :
	       (button == SDL_BUTTON_RIGHT)  ? mouse_button::right :
	       (button == SDL_BUTTON_MIDDLE) ? mouse_button::mid   :
	       (button == SDL_BUTTON_X1)     ? mouse_button::alt_0 :
	       (button == SDL_BUTTON_X2)     ? mouse_button::alt_1
	                                     : mouse_button::left;
}
}
}
