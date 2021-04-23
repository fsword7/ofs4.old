
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

namespace osd::vk
{
    class Context
    {
    public:
        Context() = default;
        ~Context() = default;

        void init();
        void cleanup();
        
        // With using SDL2 interface
        SDL_Window *createWindow();
        void destoryWundow();

    private:
        SDL_Window *window = nullptr;
    };
}