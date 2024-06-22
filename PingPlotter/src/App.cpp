
#include "App.h"


#include <iostream>
#include <mutex>
#include <string>

#include "ImGuiProgressIndicators.h"

#include "implot.h"


App::App()
{
    for (int i = 0; i < MAX_DATAPOINTS; i++) { mCurrentTime[i] = 0; mPingTimes[i] = 0; }

    mWorker.thread = std::thread(&App::Thread, this);
    mWorker.onCompleteCallback = [this] (double result, bool& completed)
        {
            completed = true;
            mPingTimes[mPingCount] = result;
            mCurrentTime[mPingCount] = mAppTimer.GetTime();

            if(mPingCount >= MAX_DATAPOINTS - 1) mPingsStarted = true;
			
            if (mPingCount >= MAX_DATAPOINTS - 1)
            {
                mPingCount = 0;
            }
            else
            {
                mPingCount++;
            }
        };

	// Signal the worker to start work
	//mWorker.workReady.notify_one();
}

App::~App()
{
    mWorker.running = false;
    mWorker.thread.join();
}

void App::Update()
{
    if (mFirstRun) { mWorker.completed = true; mFirstRun = false; }

    //mPingTimes[frameCount] = PingAddress();

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
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(),ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar);
    }

    if(!mPingsStarted)
    {
	    {
		    ImGui::Begin("Progress Indicators",nullptr, ImGuiWindowFlags_NoDecoration);

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
        // Test bar graph
        {
            ImGui::Begin("My Window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

            if (ImPlot::BeginPlot("My Plot", ImVec2{ ImGui::GetWindowViewport()->Size.x,ImGui::GetWindowViewport()->Size.y }, ImPlotFlags_CanvasOnly /*| ImPlotFlags_NoFrame*/));
            {
                ImPlot::SetupAxes("Time", "Ping", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                ImPlot::PlotLine("My Line Plot", mCurrentTime, mPingTimes, MAX_DATAPOINTS);

                ImPlot::EndPlot();
            }
            ImGui::End();
        }

        {
            ImGui::Begin("FPS");

        	ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

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
        }
    }
}