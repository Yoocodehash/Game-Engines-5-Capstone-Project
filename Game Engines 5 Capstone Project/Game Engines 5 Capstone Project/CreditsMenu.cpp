#include "CreditsMenu.h"

SDL_Event CreditsMenu::creditsEvent;

CreditsMenu::CreditsMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    if (flags_ == SDL_WINDOW_OPENGL)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GLContext mainContext = SDL_GL_CreateContext(creditsWindow);

        while (isRunning)
        {
            SDL_GL_SwapWindow(creditsWindow);
        }

        std::cout << "\nCredits menu is running in OpenGL window\n";
    }

    if (flags_ == SDL_WINDOW_VULKAN)
    {
        uint32_t extensionCount;
        SDL_Vulkan_GetInstanceExtensions(creditsWindow, &extensionCount, nullptr);
        std::vector<const char*> extensionNames(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(creditsWindow, &extensionCount, extensionNames.data());

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Flying Bird Credits";
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
        if (!SDL_Vulkan_CreateSurface(creditsWindow, instance, &surface)) {
            // failed to create a surface!
        }

        std::cout << "\nCredits menu is running in Vulkan window\n";
    }

    // Initialize window when the start button is pressed
    creditsWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    creditsRenderer = SDL_CreateRenderer(creditsWindow, -1, SDL_RENDERER_ACCELERATED);

    creditsSurface = IMG_Load("Credits.png");
    creditsTexture = SDL_CreateTextureFromSurface(creditsRenderer, creditsSurface);

    memoryPool = new MemoryPool();

    threadPool.Start(1);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    isRunning = true;
}

CreditsMenu::~CreditsMenu()
{
}

void CreditsMenu::CreditsMenuHandleEvents()
{
    while (SDL_PollEvent(&creditsEvent)) {

        switch (creditsEvent.type)
        {
        case SDL_QUIT:
            threadPool.Finish();
            memoryPool->ReleaseMemoryPool();

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (creditsEvent.key.keysym.sym)
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

void CreditsMenu::RenderCreditsMenu()
{
    SDL_RenderClear(creditsRenderer);
    SDL_RenderCopy(creditsRenderer, creditsTexture, NULL, NULL);
    SDL_RenderPresent(creditsRenderer);
}

void CreditsMenu::Clear()
{
    SDL_DestroyWindow(creditsWindow);
    SDL_DestroyRenderer(creditsRenderer);
    SDL_Quit();
}

bool CreditsMenu::Running()
{
    return isRunning;
}
