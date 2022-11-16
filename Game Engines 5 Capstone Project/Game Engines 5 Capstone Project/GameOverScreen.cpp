#include "GameOverScreen.h"

SDL_Event GameOverScreen::gameoverEvent;

GameOverScreen::GameOverScreen(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    if (flags_ == SDL_WINDOW_OPENGL)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GLContext mainContext = SDL_GL_CreateContext(gameoverWindow);

        while (isRunning)
        {
            SDL_GL_SwapWindow(gameoverWindow);
        }

        std::cout << "\nGame over screen is running in OpenGL window\n";
    }

    if (flags_ == SDL_WINDOW_VULKAN)
    {
        uint32_t extensionCount;
        SDL_Vulkan_GetInstanceExtensions(gameoverWindow, &extensionCount, nullptr);
        std::vector<const char*> extensionNames(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(gameoverWindow, &extensionCount, extensionNames.data());

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Game Over Screen";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Vulkan Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        std::vector<const char*> layerNames{};
        // uncomment below if you want to use validation layers
        //layerNames.push_back("VK_LAYER_LUNARG_standard_validation");

        VkInstanceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        info.pApplicationInfo = &appInfo;
        info.enabledLayerCount = layerNames.size();
        info.ppEnabledLayerNames = layerNames.data();
        info.enabledExtensionCount = extensionNames.size();
        info.ppEnabledExtensionNames = extensionNames.data();

        VkResult res;
        VkInstance instance;
        res = vkCreateInstance(&info, nullptr, &instance);
        if (res != VK_SUCCESS) {
            // do some error checking
        }

        VkSurfaceKHR surface;
        if (!SDL_Vulkan_CreateSurface(gameoverWindow, instance, &surface)) {
            // failed to create a surface!
        }

        std::cout << "\nGame over screen is running in Vulkan window\n";
    }

    // Initialize window when pause menu is pressed
    gameoverWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    gameoverRenderer = SDL_CreateRenderer(gameoverWindow, -1, SDL_RENDERER_ACCELERATED);

    gameoverSurface = IMG_Load("Game Over.png");
    gameoverTexture = SDL_CreateTextureFromSurface(gameoverRenderer, gameoverSurface);

    Quit.srect.y = 710;
    Quit.drect.x = 800 / 2 - Quit.drect.w / 2;
    Quit.drect.y = 450;

    memoryPool = new MemoryPool();

    //GameOverButtonsSound.LoadAudio();

    threadPool.Start(1);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    isRunning = true;
}

GameOverScreen::~GameOverScreen()
{
}

void GameOverScreen::UpdateGameOver()
{
    mouse.update();
    Quit.update(mouse);
}

void GameOverScreen::HandleEventGameOver()
{
    while (SDL_PollEvent(&gameoverEvent)) {

        switch (gameoverEvent.type)
        {
        case SDL_QUIT:
            threadPool.Finish();
            memoryPool->ReleaseMemoryPool();

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (gameoverEvent.key.keysym.sym)
            {
                // I included ESC key because if you're going to play in full screen then you can't press X and you're stuck
            case SDLK_ESCAPE:
                threadPool.Finish();
                memoryPool->ReleaseMemoryPool();

                isRunning = false;
                break;
            }

        default:
            break;
        }
    }
}

void GameOverScreen::RenderGameOver()
{
    SDL_RenderClear(gameoverRenderer);
    SDL_RenderCopy(gameoverRenderer, gameoverTexture, NULL, NULL);
    Quit.draw();
    mouse.draw();
    SDL_RenderPresent(gameoverRenderer);
}

void GameOverScreen::Clear()
{
    SDL_DestroyWindow(gameoverWindow);
    SDL_DestroyRenderer(gameoverRenderer);
}

bool GameOverScreen::Running()
{
    return isRunning;
}