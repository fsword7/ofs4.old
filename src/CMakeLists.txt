find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

set (ofs_srcs
    main/sdl2/main.cpp
    main/vulkan/command.cpp
    main/vulkan/context.cpp
    main/vulkan/image.cpp
    main/vulkan/render.cpp
    main/vulkan/sdl2.cpp
    main/vulkan/setup.cpp
    main/vulkan/swapchain.cpp
    render/scene.cpp
)

add_executable(ofs ${ofs_srcs})
target_link_libraries(ofs SDL2::SDL2 Vulkan::Vulkan)
add_definitions(-DSDL_MAIN_HANDLED)