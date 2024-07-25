#pragma once
#include <SDL3/SDL.h>

class AppIcon
{
public:
	AppIcon(SDL_Window& SDLWindow);

	AppIcon() = delete;
	AppIcon(const AppIcon&) = delete;

	~AppIcon();

private:
	void LoadWindowIcon(SDL_Window& SDLWindow);
	SDL_Surface* mIconSurface;
};
