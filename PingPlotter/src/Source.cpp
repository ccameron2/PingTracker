// Important to understand: SDL_Renderer is an _optional_ component of SDL3.
// For a multi-platform app consider using e.g. SDL+DirectX on Windows and SDL+OpenGL on Linux/OSX.

#include "imgui.h"
#include <stdio.h>

#include <SDL3/SDL.h>

#ifdef WINDOWS
#include "SDL3/SDL_main.h"
#endif

#include "App.h"
#include "implot.h"
#include "../vendor/SDL/src/render/SDL_sysrender.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

// Main code
int main(int argc, char* argv[])
{
{
	// Setup SDL
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");


	// Create window with SDL_Renderer graphics context
	Uint32 window_flags = SDL_WINDOW_RESIZABLE;

	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_CreateWindowAndRenderer("Ping Plotter", 1280, 720, window_flags, &window, &renderer);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
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
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	// Our state
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	App* app = new App(*window, argv);

	// Main loop
	bool done = false;

	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				done = true;
			if (event.type == SDL_EVENT_WINDOW_MINIMIZED)
				app->mMinimised = true;
			if (event.type == SDL_EVENT_WINDOW_MAXIMIZED)
				app->mMinimised = false;
			if (event.type == SDL_EVENT_WINDOW_RESTORED)
				app->mMinimised = false;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();

		// Mem leak here 12 bytes
		ImGui_ImplSDL3_NewFrame();
		// mem leak end

		ImGui::NewFrame();

		app->Update();

		// Rendering
		ImGui::Render();
		SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(),renderer);
		SDL_RenderPresent(renderer);
	}

	delete app;

	// Cleanup
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	ImPlot::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	SDL_Quit();
}
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

    return 0;
}
