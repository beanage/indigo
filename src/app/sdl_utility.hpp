#pragma once

#include <SDL2/SDL.h>
#include "mouse_event.hpp"

namespace indigo
{
namespace sdl_utility
{
unsigned translate_sdl_keycode(SDL_Keycode);
unsigned translate_sdl_modifiers(Uint16 mods);
mouse_button translate_sdl_mousebutton(Uint8 button);
}
}
