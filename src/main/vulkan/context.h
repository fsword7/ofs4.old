
#pragma once

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

    struct FrameEntry
    {
        VkImage image = nullptr;
        VkImageView view = nullptr;
        VkDeviceMemory memory = nullptr;
        VkFormat format = VK_FORMAT_UNDEFINED;
        VkSampler sampler = nullptr;
    };

    struct FrameAttachment
    {
        FrameEntry color;
        FrameEntry depth;
    };

    struct Vertex
    {
        float position[3];
        float color[3];
        float texcoord[2];

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription = {};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

            // position
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, position);

            // color
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            // texcoord
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 2;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, texcoord);

            return attributeDescriptions;
        }
    };

    struct sBuffer
    {
        VkBuffer buffer = nullptr;
        VkDeviceMemory memory = nullptr;
        uint32_t entries = 0;
        uint32_t size = 0;

        VkSharingMode mode;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags mpFlags;

        void *mapped = nullptr;
    };

    struct sTexture
    {
        sBuffer buffer;
        uint32_t width;
        uint32_t height;
        uint32_t size;

        VkFormat format = VK_FORMAT_UNDEFINED;
        VkImage image = nullptr;
        VkImageView view = nullptr;
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkDeviceMemory memory = nullptr;
        VkSampler sampler = nullptr;
    };

    class Context : public SDL2_Interface
    {
    public:
        Context() = default;
        ~Context() = default;

        void init();
        void cleanup();

        // Render processing section
        void startRender();
        void stopRender();
        // void beginGraphicsCommandBuffer(std::vector<VkCommandBuffer> &cmdBuffers);
        VkCommandBuffer beginRender();
        void endRender(VkCommandBuffer);
        void endRender(VkCommandBuffer cmdBuffer, VkSemaphore signal);

        inline const VkPhysicalDevice getPhysicalDevice() const { return gpuDevice; }
        inline const VkDevice getLogicalDevice() const { return device; }
        inline const VkExtent2D getSurfaceExtent() const { return surfaceImageExtent; }
        inline const std::vector<VkImageView> &getSurfaceImageViews() const { return surfaceImageViews; }
        inline const VkRenderPass getRenderPass() const { return renderPass; }
        inline const VkFramebuffer getFrameBuffer(int idx) const { return frameBuffers[idx]; }

        inline const VkFormat getSurfaceImageFormat() const { return surfaceImageFormat; }
        inline const VkFormat getColorImageFormat(int idx = 0) const { return frameAttachments[idx].color.format; }
        inline const VkFormat getDepthImageFormat(int idx = 0) const { return frameAttachments[idx].depth.format; }
        inline const VkImageView &getColorImageView(int idx = 0) const { return frameAttachments[idx].color.view; }
        inline const VkImageView &getDepthImageView(int idx = 0) const { return frameAttachments[idx].depth.view; }

        inline const VkQueue getGraphicsQueue() const { return graphicsQueue; }
        inline const VkQueue getPresentQueue() const  { return presentQueue; }
        
        uint32_t findMemoryType(const VkPhysicalDeviceMemoryProperties &props, uint32_t typeFilter,
            VkMemoryPropertyFlags mpFlags);
        void createBuffer(const VkDevice &device, const VkPhysicalDeviceMemoryProperties &props,
            VkBuffer &buffer, VkDeviceMemory &memory, VkDeviceSize size, VkBufferUsageFlags usageFlags,
            VkSharingMode sharingMode, VkMemoryPropertyFlags mpFlags);
        void createTexture(const VkPhysicalDevice &gpu, const VkDevice &device);

        void createImageSampler(const VkDevice &device, const VkImage &image, VkFormat format,
            VkImageView &view, VkSampler &sampler);
        void convertImageLayout(const VkCommandBuffer &cmdBuffer, VkImage image,
	        VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange range);

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
            VkImage &image, VkDeviceMemory &imageMemory, VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void convertImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void createAttachmentImage(VkFormat format, VkImageUsageFlags usageFlags, FrameEntry &attachment);

        void createBuffer();
        
        VkFormat findDepthFormat();

        // void readShaderFile(const std::string &fname, char *&data, uint32_t &size);
        
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
        // void createColorResources();
        // void createDepthResources();
        void createRenderPass();
        void createFrameBuffers();

        // void createPipeline();

        // void cleanupPipeline();
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
        // VkImage colorImage = nullptr;
        // VkImageView colorImageView = nullptr;
        // VkDeviceMemory colorImageMemory = nullptr;
        VkFormat colorImageFormat = VK_FORMAT_UNDEFINED;

        // depth image resources
        // VkImage depthImage = nullptr;
        // VkImageView depthImageView = nullptr;
        // VkDeviceMemory depthImageMemory = nullptr;
        VkFormat depthImageFormat = VK_FORMAT_UNDEFINED;
     
        VkRenderPass renderPass = nullptr;
        std::vector<VkFramebuffer> frameBuffers;
        std::vector<FrameAttachment> frameAttachments;
        std::vector<VkCommandBuffer> graphicsCommandBuffers;
        
        VkQueue graphicsQueue = nullptr;
        VkQueue presentQueue = nullptr;
        std::vector<VkFence> fences;
        VkSemaphore imageAvailableSemaphore = nullptr;
        VkSemaphore renderFinishedSemaphore = nullptr;

        VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo submitInfo = {};
        VkPresentInfoKHR presentInfo = {};
        uint32_t imageIndex = 0;

        // Temporary - to be removed later when multi-pipeline package is implemented
        VkPipelineLayout pipelineLayout = nullptr;
        VkPipeline graphicsPipeline = nullptr;
        VkShaderModule vertShader = nullptr;
        VkShaderModule fragShader = nullptr;

        sBuffer vtxObject;
        sBuffer idxObject;
        sTexture txObject;     
    };
}