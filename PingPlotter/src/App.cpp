#include "App.h"

#include "imgui.h"
#include <stdio.h>

#include <SDL3/SDL.h>

#ifndef WINDOWS
#include "AppIcon.h"
#endif

#include "implot.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

App::App()
{
	// Setup SDL & Imgui
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

	// Create window with SDL_Renderer graphics context
	Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;

	SDL_CreateWindowAndRenderer("Ping Plotter", 1280, 720, window_flags, &mWindow, &mRenderer);
	SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(mWindow);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	  
	io.IniFilename = "PingPlotterConfig.ini";

#ifdef _DEBUG
	io.ConfigDebugIsDebuggerPresent = true;
#endif

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(mWindow, mRenderer);
	ImGui_ImplSDLRenderer3_Init(mRenderer);

	io.Fonts->AddFontFromFileTTF("Roboto.ttf", 15.0f);
	io.Fonts->ConfigData[0].RasterizerDensity = 3.0f;
#ifndef WINDOWS
    mAppIcon = std::make_unique<AppIcon>(window);
#endif

    mAppColours = std::make_unique<AppColours>();
    mPingPlotter = std::make_unique<PingPlotter>(*mAppColours.get());
}

App::~App()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	ImPlot::DestroyContext();

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	SDL_Quit();
}

bool App::Update()
{
	bool done = false;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL3_ProcessEvent(&event);
		if (event.type == SDL_EVENT_QUIT)
			done = true;
		if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(mWindow))
			done = true;
		if (event.type == SDL_EVENT_WINDOW_MINIMIZED)
			mMinimised = true;
		if (event.type == SDL_EVENT_WINDOW_MAXIMIZED)
			mMinimised = false;
		if (event.type == SDL_EVENT_WINDOW_RESTORED)
			mMinimised = false;
	}

	ImGui_ImplSDLRenderer3_NewFrame();

	ImGui_ImplSDL3_NewFrame();

	ImGui::NewFrame();

	if (!mMinimised) mPingPlotter->Update();

	// Rendering
	ImGui::Render();

	auto& io = ImGui::GetIO();
	SDL_SetRenderScale(mRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);

	SDL_SetRenderDrawColor(mRenderer, (Uint8)(mClearColour.x * 255), (Uint8)(mClearColour.y * 255), (Uint8)(mClearColour.z * 255), (Uint8)(mClearColour.w * 255));

	SDL_RenderClear(mRenderer);

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), mRenderer);
	SDL_RenderPresent(mRenderer);

	return done;
}
