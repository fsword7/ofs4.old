
#include "main/core.h"
#include "main/vulkan/context.h"

using namespace osd::vk;

SDL_Window *Context::createWindow()
{
    window = SDL_CreateWindow(APP_SHORT, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        OFSW_WIDTH, OFSW_HEIGHT, SDL_WINDOW_VULKAN);
    SDL_ShowWindow(window);

    return window;
}

void Context::destoryWundow()
{
    SDL_DestroyWindow(window);
}