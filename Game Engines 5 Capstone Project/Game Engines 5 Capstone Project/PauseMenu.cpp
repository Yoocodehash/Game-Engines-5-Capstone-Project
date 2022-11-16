#include "PauseMenu.h"

SDL_Event PauseMenu::pauseEvent;

PauseMenu::PauseMenu(const char* title_, int x_, int y_, int w_, int h_, int flags_)
{
    if (flags_ == SDL_WINDOW_OPENGL)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GLContext mainContext = SDL_GL_CreateContext(pauseWindow);

        while (isRunning)
        {
            SDL_GL_SwapWindow(pauseWindow);
        }

        std::cout << "\nPause menu is running in OpenGL window\n";
    }

    if (flags_ == SDL_WINDOW_VULKAN)
    {
        uint32_t extensionCount;
        SDL_Vulkan_GetInstanceExtensions(pauseWindow, &extensionCount, nullptr);
        std::vector<const char*> extensionNames(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(pauseWindow, &extensionCount, extensionNames.data());

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Pause Menu";
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
        if (!SDL_Vulkan_CreateSurface(pauseWindow, instance, &surface)) {
            // failed to create a surface!
        }

        std::cout << "\nPause menu is running in Vulkan window\n";
    }

    // Initialize window when pause menu is pressed
    pauseWindow = SDL_CreateWindow(title_, x_, y_, w_, h_, flags_);
    pauseRenderer = SDL_CreateRenderer(pauseWindow, -1, SDL_RENDERER_ACCELERATED);

    pauseSurface = IMG_Load("Pause Menu.png");
    pauseTexture = SDL_CreateTextureFromSurface(pauseRenderer, pauseSurface);

    Resume.srect.y = 710;
    Resume.drect.x = 800 / 2 - Resume.drect.w / 2;
    Resume.drect.y = 250;

    Options.srect.y = 310; // This will give me the button name depending on the button's position
    Options.drect.x = 800 / 2 - Options.drect.w / 2;
    Options.drect.y = 350;

    Quit.srect.y = 710;
    Quit.drect.x = 800 / 2 - Quit.drect.w / 2;
    Quit.drect.y = 450;

    memoryPool = new MemoryPool();

    PauseMenuButtonsSound.LoadAudio();

    threadPool.Start(1);
    memoryPool->AllocatePool(10, 10, 1);
    memoryPool->AllocateAligned(13, 1);

    optionsMenu = nullptr;

    isRunning = true;
}

PauseMenu::~PauseMenu()
{

}

void PauseMenu::UpdatePauseMenu()
{
    mouse.update();
    Resume.update(mouse);
    Options.update(mouse);
    Quit.update(mouse);
}

void PauseMenu::HandleEventPauseMenu()
{
    while (SDL_PollEvent(&pauseEvent)) {

        switch (pauseEvent.type)
        {
        case SDL_QUIT:
            threadPool.Finish();
            memoryPool->ReleaseMemoryPool();

            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (pauseEvent.key.keysym.sym)
            {
                // I included ESC key because if you're going to play in full screen then you can't press X and you're stuck
            case SDLK_ESCAPE:
                threadPool.Finish();
                memoryPool->ReleaseMemoryPool();

                isRunning = false;
                break;
            }

        case SDL_MOUSEBUTTONUP:
            if (pauseEvent.button.button == SDL_BUTTON_LEFT)
            {
                if (Resume.isSelected)
                {
                    Mix_PlayChannel(-1, PauseMenuButtonsSound.ButtonPressedSound, 0);

                    threadPool.Finish();
                    memoryPool->ReleaseMemoryPool();

                    isRunning = false;
                    break;
                }

                if (Options.isSelected)
                {
                    Mix_PlayChannel(-1, PauseMenuButtonsSound.ButtonPressedSound, 0);

                    SDL_DestroyRenderer(pauseRenderer);
                    SDL_DestroyWindow(pauseWindow);

                    // Initialize window when the start button is pressed
                    optionsMenu = new OptionsMenu("Options", SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED, 800, 600, 0);

                    while (optionsMenu->Running())
                    {

                        optionsMenu->UpdateOptionsMenu();
                        optionsMenu->OptionsMenuHandleEvents();
                        optionsMenu->RenderOptionsMenu();
                    }

                    if (!optionsMenu->Running())
                    {
                        optionsMenu->Clear(); // If the options menu isn't running, then clear the options menu
                        isRunning = false; // Without this, the game will freeze
                    }

                    break;
                }
            }

        default:
            break;
        }
    }
}

void PauseMenu::RenderPauseMenu()
{
    SDL_RenderClear(pauseRenderer);
    SDL_RenderCopy(pauseRenderer, pauseTexture, NULL, NULL);
    Resume.draw();
    Options.draw();
    Quit.draw();
    mouse.draw();
    SDL_RenderPresent(pauseRenderer);
}

void PauseMenu::Clear()
{
    // Don't use SDL_Quit() because it'll quit the whole window program
    SDL_DestroyWindow(pauseWindow);
    SDL_DestroyRenderer(pauseRenderer);
}

bool PauseMenu::Running()
{
    return isRunning;
}
