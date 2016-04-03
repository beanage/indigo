#ifndef __SDL_EVENT_DISPATCHER_HPP_INCLUDED__
#define __SDL_EVENT_DISPATCHER_HPP_INCLUDED__

#include <SDL2/SDL.h>

namespace indigo
{
namespace sdl_utility
{
unsigned translate_sdl_keycode(SDL_Keycode);
unsigned translate_sdl_modifiers(Uint16 mods);
}
}

#endif
