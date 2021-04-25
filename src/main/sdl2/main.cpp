
#include "main/core.h"
#include "main/vulkan/context.h"
#include "render/scene.h"

osd::vk::Context *ctx = nullptr;
ofs::render::Scene *scene = nullptr;
SDL_Window *window = nullptr;

void init()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    ctx = new osd::vk::Context();
    window = ctx->createWindow(APP_SHORT, OFSW_WIDTH, OFSW_HEIGHT);

    ctx->init();

    scene = new ofs::render::Scene(*ctx);
}

void cleanup()
{
    if (scene != nullptr)
        delete scene;

    if (ctx != nullptr)
    {
        ctx->cleanup();
        delete ctx;
    }

    SDL_Quit();
}

void run()
{
    bool running = true;

    while(running)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
        // update();
        scene->render();
    }

}

int main(int argc, char **argv)
{
    std::cout << "Welcome to Orbital Flight Simulator" << std::endl;

    init();
    run();
    cleanup();

    return EXIT_SUCCESS;
}