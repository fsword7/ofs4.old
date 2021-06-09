find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

set (ofs_srcs
    engine/object.cpp
    main/sdl2/main.cpp
    main/vulkan/buffer.cpp
    main/vulkan/command.cpp
    main/vulkan/context.cpp
    main/vulkan/demo.cpp
    main/vulkan/image.cpp
    main/vulkan/pipeline.cpp
    main/vulkan/render.cpp
    main/vulkan/sdl2.cpp
    main/vulkan/shader.cpp
    main/vulkan/setup.cpp
    main/vulkan/swapchain.cpp
    main/vulkan/texture.cpp
    render/pipelines/demo.cpp
    render/pipelines/star.cpp
    render/scene.cpp
    universe/object.cpp
    universe/surface.cpp
    utils/sphere.cpp
)

add_executable(ofs ${ofs_srcs})
target_link_libraries(ofs SDL2::SDL2 Vulkan::Vulkan)
add_definitions(-DSDL_MAIN_HANDLED)