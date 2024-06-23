
#include "App.h"


#include <iostream>
#include <mutex>
#include <string>

#include "ImGuiProgressIndicators.h"

#include "implot.h"


App::App()
{
    mCurrentTime = new float[MAX_DATAPOINTS];
    mPingTimes = new float[MAX_DATAPOINTS];

    for (int i = 0; i < MAX_DATAPOINTS; i++) { mCurrentTime[i] = 0; mPingTimes[i] = 0; }

    mWorker.thread = std::thread(&App::Thread, this);
    mWorker.onCompleteCallback = [this] (double result, bool& completed)
        {
            completed = true;
            mPingTimes[mPingCount] = result;
            mCurrentTime[mPingCount] = mAppTimer.GetTime();

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

	// Signal the worker to start work
	//mWorker.workReady.notify_one();
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
            ImGui::Begin("Ping Plotter", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

            if (ImPlot::BeginPlot("My Plot", ImVec2{ -1,-1 }, ImPlotFlags_CanvasOnly /*| ImPlotFlags_NoFrame*/));
            {
                ImPlot::SetupAxes("Time", "Ping", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);

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

                delete currentTimeDisplay;
                delete pingTimesDisplay;

                ImPlot::EndPlot();
            }
            ImGui::End();
        }

        {
            static bool showDemoWindow = true;
            ImGui::ShowDemoWindow(&showDemoWindow);
        }

        // Control panel
        {
            ImGui::Begin("Controls");

            static bool showAllData = false;
            if(ImGui::Checkbox("View All", &showAllData))
            {
                mMaxDataDisplay = INITIAL_DATA_TO_VIEW;
            }

            ImGui::SameLine();

            if (ImGui::Button("Clear"))
            {
                ClearVisualiser();
            }

            // Max data limit slider
            if (showAllData)
            {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            	mMaxDataDisplay = mPingCount;
            }

            ImGui::PushItemWidth(ImGui::GetWindowWidth());
            ImGui::SliderInt("Max data", &mMaxDataDisplay, 5, mPingCount > INITIAL_DATA_TO_VIEW ? mPingCount : INITIAL_DATA_TO_VIEW);
            

            if (showAllData)
            {
                ImGui::PopItemFlag();
				ImGui::PopStyleVar();
            }
            ////

        	ImGuiIO& io = ImGui::GetIO(); 
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::Text("Average ping: %.2f ms", cumulativePing / dataDisplay);

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

void App::ClearVisualiser()
{
    mPingCount = 0;
}
