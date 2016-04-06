#pragma once

#include <SDL2/SDL.h>

namespace indigo
{
namespace sdl_utility
{
unsigned translate_sdl_keycode(SDL_Keycode);
unsigned translate_sdl_modifiers(Uint16 mods);
}
}
