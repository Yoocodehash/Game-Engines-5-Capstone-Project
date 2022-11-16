#include "LevelCompleteScreen.h"

SDL_Event LevelCompleteScreen::LevelCompleteEvent;

LevelCompleteScreen::LevelCompleteScreen(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    if (flags_ == SDL_WINDOW_OPENGL)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GLContext mainContext = SDL_GL_CreateContext(LevelCompleteWindow);

        while (isRunning)
        {
            SDL_GL_SwapWindow(LevelCompleteWindow);
        }

        std::cout << "\nLevel complete screen is running in OpenGL window\n";
    }

    if (flags_ == SDL_WINDOW_VULKAN)
    {
        uint32_t extensionCount;
        SDL_Vulkan_GetInstanceExtensions(LevelCompleteWindow, &extensionCount, nullptr);
        std::vector<const char*> extensionNames(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(LevelCompleteWindow, &extensionCount, extensionNames.data());

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Level Complete Screen";
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
        if (!SDL_Vulkan_CreateSurface(LevelCompleteWindow, instance, &surface)) {
            // failed to create a surface!
        }

        std::cout << "\nLevel complete screen is running in Vulkan window\n";
    }

    // Initialize window when pause menu is pressed
    LevelCompleteWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    LevelCompleteRenderer = SDL_CreateRenderer(LevelCompleteWindow, -1, SDL_RENDERER_ACCELERATED);

    LevelCompleteSurface = IMG_Load("Level Complete.png");
    LevelCompleteTexture = SDL_CreateTextureFromSurface(LevelCompleteRenderer, LevelCompleteSurface);

    Exit.srect.y = 710;
    Exit.drect.x = 100 / 2 - Exit.drect.w / 2;
    Exit.drect.y = 450;

    memoryPool = new MemoryPool();

    ButtonSound.LoadAudio();

    threadPool.Start(1);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    isRunning = true;
}

LevelCompleteScreen::~LevelCompleteScreen()
{
}

void LevelCompleteScreen::UpdateLevelCompleteScreen()
{
    mouse.update();
    Exit.update(mouse);
}

void LevelCompleteScreen::HandleEventLevelCompleteScreen()
{
    while (SDL_PollEvent(&LevelCompleteEvent)) {

        switch (LevelCompleteEvent.type)
        {
        case SDL_QUIT:
            threadPool.Finish();
            memoryPool->ReleaseMemoryPool();

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (LevelCompleteEvent.key.keysym.sym)
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

void LevelCompleteScreen::RenderLevelCompleteScreen()
{
    SDL_RenderClear(LevelCompleteRenderer);
    SDL_RenderCopy(LevelCompleteRenderer, LevelCompleteTexture, NULL, NULL);
    Exit.draw();
    mouse.draw();
    SDL_RenderPresent(LevelCompleteRenderer);
}

void LevelCompleteScreen::Clear()
{
    // Don't use SDL_Quit() because it'll quit the whole window program
    SDL_DestroyWindow(LevelCompleteWindow);
    SDL_DestroyRenderer(LevelCompleteRenderer);
    SDL_Quit();
}

bool LevelCompleteScreen::Running()
{
    return isRunning;
}
