#pragma once

#include "imgui.h"

#include "AppColours.h"
#include "PingTracer/PingTracer.h"

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
	bool mMaximised = false;
	ImVec4 mClearColour = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGuiIO* mIO;
	std::unique_ptr<PingTracer> mPingTracer;
};