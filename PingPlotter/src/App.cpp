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

App::App(SDL_Window* window)
{
    mSDLWindow = window;

    mAppTimer.start();
    mCurrentTime = new float[MAX_DATAPOINTS];
    mPingTimes = new float[MAX_DATAPOINTS];
    LoadWindowIcon("PingPlotter.png"); // load from header
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
    mWorker.running = false;
    mWorker.thread.join();
    delete[] mCurrentTime;
    delete[] mPingTimes;
}

void App::Update()
{
    if (mFirstRun)
    {
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
        int dataDisplay = mMaxDataDisplay;

        // Ping / time line graph
        {
#ifdef _DEBUG
            ImGui::Begin("Ping Plotter", nullptr, ImGuiWindowFlags_NoDecoration);
#else
            ImGui::Begin("Ping Plotter", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

#endif

            if (ImPlot::BeginPlot("My Plot", ImVec2{ -1,-1 }, ImPlotFlags_CanvasOnly /*| ImPlotFlags_NoFrame*/));
            {
                ImPlot::SetupAxes("Time (s)", "Ping (ms)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);

                // Get segment of data to display from data arrays
                if (mPingCount < mMaxDataDisplay)
                {
                    dataDisplay = mPingCount;
                }
                else
                {
                    dataDisplay = mMaxDataDisplay;
                }

                auto* currentTimeDisplay = new float[mMaxDataDisplay];
                auto* pingTimesDisplay = new float[mMaxDataDisplay];

                if(mPingCount > dataDisplay)
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

                ImPlot::PlotLine("My Line Plot", currentTimeDisplay, pingTimesDisplay, dataDisplay);

                delete[] currentTimeDisplay;
                delete[] pingTimesDisplay;

                ImPlot::EndPlot();
            }
            ImGui::End();
        }

        {
           /* static bool showDemoWindow = true;
            ImGui::ShowDemoWindow(&showDemoWindow);*/
        }

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

            ImGui::SameLine();

            if (ImGui::Checkbox("Dark mode", &mDarkMode))
            {
                if (mDarkMode) ImGui::StyleColorsDark();
                else ImGui::StyleColorsLight();
            }

            // Max data limit slider
            if (mShowAllData)
            {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            	mMaxDataDisplay = mPingCount;
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
            ImGui::Text("Average ping: %.2f ms", cumulativePing / dataDisplay);
//#ifdef _DEBUG
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//#endif
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
}

enum
{
    kPixelSize = 4,
    kWidth = 256,
    kHeight = 256,
    kResolution = kWidth * kHeight,
    kPixelBufferSize = kPixelSize * kResolution
};

// this leaks 
void App::LoadWindowIcon()
{

    unsigned char pixelBuffer[kPixelBufferSize];

    //std::filesystem::path filepath = fileName;

    //filepath = fileName;
    //if (filepath.is_relative())
    //{
    //    filepath = std::filesystem::current_path() / filepath;
    //}
    //
    //std::string path = filepath.string();
    int width, height, numComponents;

    const bin2cpp::File& resource = bin2cpp::getIconPngFile();
    auto pixelCPPData = resource.getBuffer();
    auto pixelCPPDataUnsigned = (const stbi_uc*)pixelCPPData;
    unsigned char* pixelData = stbi_load_from_memory(pixelCPPDataUnsigned, sizeof(pixelBuffer) , &width, &height, &numComponents, 4);
   // unsigned char* pixelData = stbi_load(path.c_str(), &width, &height, &numComponents, 4);

    memcpy(pixelBuffer, pixelData, sizeof(pixelBuffer));

    SDL_Surface* rgbSurface = SDL_CreateSurface(kWidth, kHeight, SDL_PIXELFORMAT_RGBA32);

    SDL_LockSurface(rgbSurface);

    for (int y = 0; y < kHeight; ++y)
    {
        for (int x = 0; x < kWidth; ++x)
        {
            Uint32* pixel = nullptr;
            int surfaceBytesPerPixel = rgbSurface->format->bytes_per_pixel;
            int surfaceRowSize = rgbSurface->pitch;
            int surfaceOffset = x * surfaceBytesPerPixel + y * surfaceRowSize;
            Uint8* pixels = (Uint8*)rgbSurface->pixels;
            SDL_PixelFormat* pixelFormat = rgbSurface->format;

            int pixelBufferOffset = 4 * (x + kWidth * y);
            unsigned char kRed = pixelBuffer[pixelBufferOffset];
            unsigned char kGreen = pixelBuffer[pixelBufferOffset + 1];
            unsigned char kBlue = pixelBuffer[pixelBufferOffset + 2];
            unsigned char kAlpha = pixelBuffer[pixelBufferOffset + 3];

            Uint32 colour =
                SDL_MapRGBA(pixelFormat, kRed, kGreen, kBlue, kAlpha);

            pixel = (Uint32*)(pixels + surfaceOffset);
            *pixel = colour;
        }
    }

    SDL_UnlockSurface(rgbSurface);

	//stbi_image_free(pixelData);

    SDL_SetWindowIcon(mSDLWindow,rgbSurface);
}