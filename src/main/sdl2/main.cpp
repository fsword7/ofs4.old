
#include "main/core.h"
#include "main/vulkan/context.h"

osd::vk::Context *ctx = nullptr;
SDL_Window *window = nullptr;

void init()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    ctx = new osd::vk::Context();
    window = ctx->createWindow();

    ctx->init();
}

void cleanup()
{
    ctx->cleanup();
    ctx->destoryWundow();
    delete ctx;

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
        // render();
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