#pragma once

#include "imgui.h"

#include "AppIcon.h"
#include "AppColours.h"
#include "PingPlotter/PingPlotter.h"

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

	std::unique_ptr<AppIcon> mAppIcon;
	std::unique_ptr<AppColours> mAppColours;
	std::unique_ptr<PingPlotter> mPingPlotter;
};