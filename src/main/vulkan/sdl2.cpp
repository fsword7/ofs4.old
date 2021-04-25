
#include "main/core.h"
#include "main/vulkan/context.h"

using namespace osd::vk;

SDL_Window *SDL2_Interface::createWindow(cchar_t *appName, int width, int height)
{
    window = SDL_CreateWindow(appName, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_VULKAN /* | SDL_WINDOW_RESIZABLE */);
    SDL_ShowWindow(window);

    return window;
}

void SDL2_Interface::destroyWindow()
{
    SDL_DestroyWindow(window);
}

void SDL2_Interface::createSurface(VkInstance instance)
{
    if (!SDL_Vulkan_CreateSurface(window, instance, &surface))
        throw std::runtime_error("Can't create surface - aborted!");
}

void SDL2_Interface::getWindowSize(int &width, int &height)
{
    SDL_Vulkan_GetDrawableSize(window, &width, &height);
}