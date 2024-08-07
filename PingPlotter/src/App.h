#pragma once

#include "imgui.h"

#include "AppColours.h"
#include "PingPlotter.h"

#ifndef WINDOWS
#include "AppIcon.h"
#endif

class App
{
public:
	App();
	~App();

	bool Update();
private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	bool mMinimised = false;
	ImVec4 mClearColour = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGuiIO* mIO;

#ifndef WINDOWS
	std::unique_ptr<AppIcon> mAppIcon;
#endif

	std::unique_ptr<AppColours> mAppColours;
	std::unique_ptr<PingPlotter> mPingPlotter;
};