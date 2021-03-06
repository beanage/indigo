#include "window.hpp"
#include "gl.hpp"

#include <SDL2/SDL.h>

using namespace indigo;

static void glew_init()
{
#if !__APPLE__
    glewExperimental = GL_TRUE;
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK)
        throw std::runtime_error("[glew_init] error");
#endif
}

struct window::impl
{
    SDL_Window* wnd;
    SDL_GLContext cxt;
};

window::window(const irect& rect)
    : impl_(new impl)
{
    impl_->wnd =
        SDL_CreateWindow("", rect.left, rect.top, rect.width(), rect.height(),
                         SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!impl_->wnd)
        throw std::runtime_error("SDL_CreateWindow failed!");

    impl_->cxt = SDL_GL_CreateContext(impl_->wnd);

    if (!impl_->cxt)
        throw std::runtime_error("SDL_GL_CreateContext failed!");

    glew_init();
}

window::~window()
{
    SDL_GL_DeleteContext(impl_->cxt);
    SDL_DestroyWindow(impl_->wnd);
}

std::string window::title() const
{
    return SDL_GetWindowTitle(impl_->wnd);
}

window& window::title(const std::string& t)
{
    SDL_SetWindowTitle(impl_->wnd, t.c_str());
    return *this;
}

irect window::frame() const
{
    irect rect;
    SDL_GetWindowPosition(impl_->wnd, &rect.left, &rect.top);

    irect::type width, height;
    SDL_GetWindowSize(impl_->wnd, &width, &height);

    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    return rect;
}

window& window::frame(const irect& f)
{
    SDL_SetWindowPosition(impl_->wnd, f.left, f.top);
    SDL_SetWindowSize(impl_->wnd, f.width(), f.height());
    return *this;
}

void window::swap() const
{
    SDL_GL_SwapWindow(impl_->wnd);
}
