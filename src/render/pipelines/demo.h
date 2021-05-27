
#pragma once

#include "main/vulkan/pipeline.h"

namespace ofs::render
{
    using namespace osd::vk;

    class PipelineTriangle : public Pipeline
    {
    public:
        PipelineTriangle() = default;
        PipelineTriangle(Context *ctx) : Pipeline(ctx) {}
        ~PipelineTriangle() { cleanup(); };

        void init() override;
        void cleanup() override;

        // void recordCommands(const VkCommandBuffer &cmdBuffer) override;
        void render(const VkCommandBuffer &cmdBuffer) override;

    private:
        sBuffer vtxObject;
        sBuffer idxObject;
    };
}