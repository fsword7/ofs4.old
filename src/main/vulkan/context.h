
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

namespace osd::vk
{
    class SDL2_Interface
    {
    public:
        SDL2_Interface() = default;
        ~SDL2_Interface()
        {
            destroyWindow();
        }

        SDL_Window *createWindow(cchar_t *appName, int width, int height);
        void createSurface(VkInstance instance);
        void destroyWindow();

    protected:
        SDL_Window *window = nullptr;
        VkSurfaceKHR surface = nullptr;
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class Context : public SDL2_Interface
    {
    public:
        Context() = default;
        ~Context() = default;

        void init();
        void cleanup();

    private:
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice gpuDevice);

        // Physical GPU function setup calls
        void createInstance();
        void getPhysicalGPUDevice();
        void createLogicalDevice();

        // Swapchain function setup calls

    private:
        VkInstance instance = nullptr;
        VkPhysicalDevice gpuDevice = nullptr;
        VkDevice device = nullptr;

        VkQueue graphicsQueue;
        VkQueue presentQueue;
    };
}