
#include "main/core.h"
#include "main/vulkan/context.h"
#include "main/vulkan/shader.h"

using namespace osd::vk;

bool Shader::readShaderFile(fs::path fname, code_t &code)
{
    std::ifstream file(fname, std::ios::ate|std::ios::binary);

    if (!file.is_open())
        return false;

    off_t size = file.tellg();
    code.resize(size);

    // Loading shader image
    file.seekg(0);
    file.read(code.data(), size);
    file.close();

    return true;
}