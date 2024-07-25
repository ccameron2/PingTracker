#include "App.h"

App::App(SDL_Window& window, char** argv)
{

#ifndef WINDOWS
    mAppIcon = std::make_unique<AppIcon>(window);
#endif

    mAppColours = std::make_unique<AppColours>();
    mPingPlotter = std::make_unique<PingPlotter>();
}

void App::Update()
{
    if (!mMinimised) mPingPlotter->Update();
}



