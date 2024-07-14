#include "App.h"

#include <filesystem>
#include <iostream>
#include <mutex>

#include "ImGuiProgressIndicators.h"

#include "implot.h"

#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#include "stb_image.h"

#include <fstream>

#include "Icon.h"


void App::SetColours()
{
    auto& colors = ImGui::GetStyle().Colors;
    //colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    colors[ImGuiCol_Header] = mCustomColour;
    colors[ImGuiCol_HeaderHovered] = mCustomColourFull;
    colors[ImGuiCol_HeaderActive] = mCustomColourFull;

    colors[ImGuiCol_Button] = mCustomColour;
    colors[ImGuiCol_ButtonHovered] = mCustomColourFull;
    colors[ImGuiCol_ButtonActive] = mCustomColourFull;

    colors[ImGuiCol_FrameBg] = mCustomBackgroundColour;
    colors[ImGuiCol_FrameBgHovered] = mCustomBackgroundColour;
    colors[ImGuiCol_FrameBgActive] = mCustomBackgroundColour;

    colors[ImGuiCol_Tab] = mCustomColour;
	colors[ImGuiCol_TabHovered] = mCustomColourFull;
	colors[ImGuiCol_TabActive] = mCustomColourFull;
    colors[ImGuiCol_TabUnfocused] = mCustomColourDimmer;
    colors[ImGuiCol_TabUnfocusedActive] = mCustomColourDim;

    colors[ImGuiCol_TitleBg] = mCustomColour;
	colors[ImGuiCol_TitleBgActive] = mCustomColourFull;
	colors[ImGuiCol_TitleBgCollapsed] = mCustomColourFull;

    colors[ImGuiCol_DockingPreview] = mCustomColour;
    colors[ImGuiCol_DockingEmptyBg] = mCustomColour;

    colors[ImGuiCol_CheckMark] = mCustomColour;
    colors[ImGuiCol_SliderGrab] = mCustomColour;
    colors[ImGuiCol_SliderGrabActive] = mCustomColourFull;

    colors[ImGuiCol_ResizeGrip] = mCustomColour;
    colors[ImGuiCol_ResizeGripActive] = mCustomColourFull;
	colors[ImGuiCol_ResizeGripHovered] = mCustomColourFull;

	colors[ImGuiCol_SeparatorActive] = mCustomColourFull;
    colors[ImGuiCol_SeparatorHovered] = mCustomColour;

    colors[ImGuiCol_TextLink] = mCustomColour;
    colors[ImGuiCol_TextSelectedBg] = mCustomColourFull;

    colors[ImGuiCol_PlotLines] = mCustomColour;
    colors[ImGuiCol_PlotLines] = mCustomColourFull;

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 4.0f;

}


App::App(SDL_Window* window)
{
    mSDLWindow = window;

    mAppTimer.start();
    mCurrentTime = new float[MAX_DATAPOINTS];
    mPingTimes = new float[MAX_DATAPOINTS];
    LoadWindowIcon(); // load from header
    for (int i = 0; i < MAX_DATAPOINTS; i++) { mCurrentTime[i] = 0; mPingTimes[i] = 0; }

    mWorker.thread = std::thread(&App::Thread, this);
    mWorker.onCompleteCallback = [this] (double result, bool& completed)
        {
            completed = true;
            mPingTimes[mPingCount] = result;
            mCurrentTime[mPingCount] = mAppTimer.get_elapsed_ms()  / 1000.0f;

            mPingsStarted = true;
			
            if (mPingCount >= MAX_DATAPOINTS - 1)
            {
                mPingCount = 0;
            }
            else
            {
                mPingCount++;
            }
        };
}

App::~App()
{
    SDL_DestroySurface(mIconSurface);
    mWorker.running = false;
    mWorker.thread.join();
    delete[] mCurrentTime;
    delete[] mPingTimes;
}

void App::Update()
{
    if (mFirstRun)
    {
        SetColours();

	    mWorker.completed = true;
    	mFirstRun = false;
    }

	RenderAppUI();
}

double App::PingAddress()
{
    icmplib::IPAddress ipAddress("8.8.8.8");
    return icmplib::Ping(ipAddress, ICMPLIB_TIMEOUT_1S).delay;
}

void App::RenderAppUI()
{
    // Make the background a DockSpace
    {
#ifdef _DEBUG
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(),ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar);
#else
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoResize);
#endif
    }

    if(!mPingsStarted)
    {
	    {
		    ImGui::Begin("Progress Indicator",nullptr, ImGuiWindowFlags_NoDecoration);

	    	const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
	    	const ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);

	    	auto viewport = ImGui::GetMainViewport();

	    	const float radius = ImGui::GetMainViewport()->Size.y / 4;
	    	const float windowWidth = viewport->Size.x;
	    	const float windowHeight = viewport->Size.y;
	    	const float offsetX = (windowWidth / 2) - radius;
	    	const float offsetY = (windowHeight / 2) - radius;

	    	ImGui::Spinner("Loading Spinner", radius, 30, col, offsetX, offsetY);

	    	ImGui::End();
	    }
    }
    else
    {
        float cumulativePing = 0;

        //{
        //    static bool showDemoWindow = false;
        //    ImGui::ShowDemoWindow(&showDemoWindow);
        //}

        // Control panel
        {
#ifdef _DEBUG
            ImGui::Begin("Controls");
#else
            ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove );
#endif
            if(ImGui::Checkbox("View All", &mShowAllData))
            {
                mMaxDataDisplay = INITIAL_DATA_TO_VIEW;
            }

            ImGui::SameLine();

            if (ImGui::Button("Clear"))
            {
                ClearVisualiser();
            }

            ImGui::SameLine();
            ImGui::PushItemWidth(mIntervalBoxWidth);

            if (ImGui::InputInt("Interval", &mThreadSleepTime,-1,-1))
            {
                if (mThreadSleepTime > MAX_INTERVAL_MS) mThreadSleepTime = MAX_INTERVAL_MS;
                if (mThreadSleepTime < MIN_INTERVAL_MS) mThreadSleepTime = MIN_INTERVAL_MS;
            }

            // Max data limit slider
            if (mShowAllData)
            {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

            }

            ImGui::PushItemWidth(ImGui::GetWindowWidth());
            ImGui::SliderInt("Max data", &mMaxDataDisplay, 5, mPingCount > INITIAL_DATA_TO_VIEW ? mPingCount : INITIAL_DATA_TO_VIEW);
            

            if (mShowAllData)
            {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
            ////

        	ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("Average ping: %.2f ms", mCumulativePing / mPingCount);
#ifdef _DEBUG
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
#endif
            ImGui::End();
        }

        int dataDisplay;

        // Ping / time line graph
        {
#ifdef _DEBUG
            ImGui::Begin("Ping Plotter", nullptr, ImGuiWindowFlags_NoDecoration);
#else
            ImGui::Begin("Ping Plotter", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

#endif
            if (!mMinimised)
            {
                if (ImPlot::BeginPlot("My Plot", ImVec2{ -1,-1 }, ImPlotFlags_CanvasOnly /*| ImPlotFlags_NoFrame*/));
                {
                    ImPlot::SetupAxes("Time (s)", "Ping (ms)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);

                    // Get segment of data to display from data arrays
                    if (mPingCount < mMaxDataDisplay || mShowAllData)
                    {
                        dataDisplay = mPingCount;
                    }
                    else
                    {
                        dataDisplay = mMaxDataDisplay;
                    }

                    auto* currentTimeDisplay = new float[dataDisplay];
                    auto* pingTimesDisplay = new float[dataDisplay];

                    if (mPingCount >= dataDisplay)
                    {
                        for (int i = 0; i < dataDisplay; i++)
                        {
                            auto offset = mPingCount - dataDisplay;
                            currentTimeDisplay[i] = mCurrentTime[i + offset];
                            pingTimesDisplay[i] = mPingTimes[i + offset];
                            cumulativePing += pingTimesDisplay[i];
                        }
                    }
                    else
                    {
                        for (int i = 0; i < dataDisplay; i++)
                        {
                            currentTimeDisplay[i] = mCurrentTime[i];
                            pingTimesDisplay[i] = mPingTimes[i];
                            cumulativePing += pingTimesDisplay[i];
                        }
                    }
                    mCumulativePing = cumulativePing;

                    ImPlot::PushStyleColor(ImPlotCol_Line, mCustomColour);

                    ImPlot::PlotLine("My Line Plot", currentTimeDisplay, pingTimesDisplay, dataDisplay);

                    ImPlot::PopStyleColor();

                    delete[] currentTimeDisplay;
                    delete[] pingTimesDisplay;

                    ImPlot::EndPlot();
                }
            }
            ImGui::End();
            }

    }

}

void App::Thread()
{
    while (mWorker.running)
    {
        if(mWorker.completed)
        {
            mWorker.completed = false;
            mWorker.onCompleteCallback(PingAddress(),mWorker.completed);
            std::this_thread::sleep_for(std::chrono::milliseconds(mThreadSleepTime));
        }
    }
}

void App::ClearVisualiser()
{
    mPingCount = 0;
    mCumulativePing = 0;
}

void App::LoadWindowIcon()
{
    // this leaks
	const bin2cpp::File& resource = bin2cpp::getIconPngFile();
    //

    auto pixelCPPData = resource.getBuffer();
    auto pixelCPPDataUnsigned = (const stbi_uc*)pixelCPPData;

    int width, height, numComponents;

    unsigned char* pixelData = stbi_load_from_memory(pixelCPPDataUnsigned, resource.getSize(), &width, &height, &numComponents, 4);

    mIconSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);

    SDL_LockSurface(mIconSurface);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Uint32* pixel;
            int surfaceBytesPerPixel = mIconSurface->format->bytes_per_pixel;
            int surfaceRowSize = mIconSurface->pitch;
            int surfaceOffset = x * surfaceBytesPerPixel + y * surfaceRowSize;
            Uint8* pixels = (Uint8*)mIconSurface->pixels;
            SDL_PixelFormat* pixelFormat = mIconSurface->format;

            int pixelBufferOffset = 4 * (x + width * y);
            unsigned char red =   pixelData[pixelBufferOffset];
            unsigned char green = pixelData[pixelBufferOffset + 1];
            unsigned char blue =  pixelData[pixelBufferOffset + 2];
            unsigned char alpha = pixelData[pixelBufferOffset + 3];

            Uint32 colour = SDL_MapRGBA(pixelFormat, red, green, blue, alpha);

            pixel = (Uint32*)(pixels + surfaceOffset);
            *pixel = colour;
        }
    }

    SDL_UnlockSurface(mIconSurface);

	stbi_image_free(pixelData);

    SDL_SetWindowIcon(mSDLWindow,mIconSurface);
}
