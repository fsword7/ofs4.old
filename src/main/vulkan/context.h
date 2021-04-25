
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

        void getWindowSize(int &width, int &height);

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

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabiliies;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
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
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice gpuDevice, VkSurfaceKHR surface);
        VkSurfaceFormatKHR chooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR chooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D chooseSwapChainExtent(const VkSurfaceCapabilitiesKHR capabilities);

        bool hasStencilComponent(VkFormat format);
        VkFormat findSupportedFormats(std::vector<VkFormat> candidates,
            VkImageTiling tiling, VkFormatFeatureFlags features);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags mpFlags, VkDeviceSize memSize);
        void createImage(uint32_t width, uint32_t height, VkFormat format,
            VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags mpFlags,
            VkImage &image, VkDeviceMemory &imageMemory);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void convertImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

        VkFormat findDepthFormat();

        VkCommandBuffer beginSingleTimeCommands(VkCommandPool cmdPool);
        void endSingleTimeCommands(VkCommandBuffer cmdBuffer, VkCommandPool cmdPool);

        // Physical GPU function setup calls
        void createInstance();
        void getPhysicalGPUDevice();
        void createLogicalDevice();
        void createCommandPool();

        // Swapchain function setup calls
        void createSwapChain();
        void createImageViews();
        void createColorResources();
        void createDepthResources();
        void createRenderPass();

        void cleanupSwapChain(bool remove);

    private:
        VkInstance instance = nullptr;
        VkPhysicalDevice gpuDevice = nullptr;
        VkDevice device = nullptr;
        VkCommandPool cmdPool = nullptr;
        QueueFamilyIndices indices;

        VkSwapchainKHR swapChain = nullptr;

        // surface image resources for swap chain process
        VkExtent2D surfaceImageExtent = { 0, 0 };
        std::vector<VkImage> surfaceImages;
        std::vector<VkImageView> surfaceImageViews;
        VkFormat surfaceImageFormat = VK_FORMAT_UNDEFINED;

        // color (immediate) image resources
        VkImage colorImage = nullptr;
        VkImageView colorImageView = nullptr;
        VkDeviceMemory colorImageMemory = nullptr;
        VkFormat colorImageFormat = VK_FORMAT_UNDEFINED;

        // depth image resources
        VkImage depthImage = nullptr;
        VkImageView depthImageView = nullptr;
        VkDeviceMemory depthImageMemory = nullptr;
        VkFormat depthImageFormat = VK_FORMAT_UNDEFINED;
     
        VkRenderPass renderPass = nullptr;
        
        VkQueue graphicsQueue = nullptr;
        VkQueue presentQueue = nullptr;
    };
}