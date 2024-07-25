#pragma once

#include "imgui.h"

#include "AppIcon.h"
#include "AppColours.h"
#include "PingPlotter/PingPlotter.h"

class App
{
public:
	App(SDL_Window& window, char** argv);

	void Update();

	double PingAddress();
	void RenderAppUI();
	ImVec4 GetClearColour() { return clear_color; }
	bool mMinimised = false;

private:
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::unique_ptr<AppIcon> mAppIcon;
	std::unique_ptr<AppColours> mAppColours;

	std::unique_ptr<PingPlotter> mPingPlotter;
};




