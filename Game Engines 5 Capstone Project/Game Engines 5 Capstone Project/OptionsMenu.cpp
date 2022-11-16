#include "OptionsMenu.h"

SDL_Event OptionsMenu::optionsEvent;

OptionsMenu::OptionsMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    if (flags_ == SDL_WINDOW_OPENGL)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GLContext mainContext = SDL_GL_CreateContext(optionsWindow);

        while (isRunning)
        {
            SDL_GL_SwapWindow(optionsWindow);
        }

        std::cout << "\nOptions menu is running in OpenGL window\n";
    }

    if (flags_ == SDL_WINDOW_VULKAN)
    {
        uint32_t extensionCount;
        SDL_Vulkan_GetInstanceExtensions(optionsWindow, &extensionCount, nullptr);
        std::vector<const char*> extensionNames(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(optionsWindow, &extensionCount, extensionNames.data());

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Options Menu";
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
        if (!SDL_Vulkan_CreateSurface(optionsWindow, instance, &surface)) {
            // failed to create a surface!
        }

        std::cout << "\nOptions menu is running in Vulkan window\n";
    }

    // Initialize window when the start button is pressed
    optionsWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    optionsRenderer = SDL_CreateRenderer(optionsWindow, -1, SDL_RENDERER_ACCELERATED);

    optionsSurface = IMG_Load("Options Menu.png");
    optionsTexture = SDL_CreateTextureFromSurface(optionsRenderer, optionsSurface);

    memoryPool = new MemoryPool();

    threadPool.Start(1);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    exitButton.srect.y = 710;
    exitButton.drect.x = 800 / 2 - exitButton.drect.w / 2;
    exitButton.drect.y = 550;

    isRunning = true;
}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::UpdateOptionsMenu()
{
    mouse.update();
    exitButton.update(mouse);
}

void OptionsMenu::OptionsMenuHandleEvents()
{
    while (SDL_PollEvent(&optionsEvent)) {

        switch (optionsEvent.type)
        {
        case SDL_QUIT:
            threadPool.Finish();
            memoryPool->ReleaseMemoryPool();

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (optionsEvent.key.keysym.sym)
            {
                // I included ESC key because if you're going to play in full screen then you can't press X and you're stuck
            case SDLK_ESCAPE:
                threadPool.Finish();
                memoryPool->ReleaseMemoryPool();

                isRunning = false;
                break;
            }

        case SDL_MOUSEBUTTONUP:
                if (optionsEvent.button.button == SDL_BUTTON_LEFT)
                {
                    if (exitButton.isSelected)
                    {
                        threadPool.Finish();
                        memoryPool->ReleaseMemoryPool();

                        isRunning = false;
                        break;
                    }
                }

        default:
            break;
        }
    }
}

void OptionsMenu::RenderOptionsMenu()
{
    SDL_RenderClear(optionsRenderer);
    SDL_RenderCopy(optionsRenderer, optionsTexture, NULL, NULL);
    exitButton.draw();
    mouse.draw();
    SDL_RenderPresent(optionsRenderer);
}

void OptionsMenu::Clear()
{
    // Don't use SDL_Quit() because it'll quit the whole window program
    SDL_DestroyWindow(optionsWindow);
    SDL_DestroyRenderer(optionsRenderer);
}

bool OptionsMenu::Running()
{
    return isRunning;
}