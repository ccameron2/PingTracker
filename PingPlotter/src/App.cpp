
#include "App.h"


#include <iostream>
#include <mutex>
#include <string>


#include "implot.h"


App::App()
{
    for (int i = 0; i < MAX_DATAPOINTS; i++) { mCurrentTime[i] = 0; mPingTimes[i] = 0; }

    mWorker.thread = std::thread(&App::Thread, this);
    mWorker.onCompleteCallback = [this] (double result, bool& completed)
        {
            completed = true;
            mPingTimes[pingCount] = result;
            mCurrentTime[pingCount] = appTimer.GetTime();

            if (pingCount + 1 < MAX_DATAPOINTS)
            {
                pingCount++;
            }
            else
            {
                pingCount = 0;
            }
        };

	// Signal the worker to start work
	//mWorker.workReady.notify_one();
}

App::~App()
{
    mWorker.thread.join();
}

void App::Update()
{
    if (first) { mWorker.completed = true; first = false; }

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
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    }


    // Test bar graph
	{
		ImGui::Begin("My Window");
    	if (ImPlot::BeginPlot("My Plot"))
    	{
    		ImPlot::PlotLine("My Line Plot", mCurrentTime, mPingTimes, MAX_DATAPOINTS);
    		ImPlot::EndPlot();
    	}
    	ImGui::End();
	}


    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        if (ImGui::SliderFloat("float", &f, 0.0f, 1.0f))
        {
            // This will run when above is changed
        };


        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGuiIO& io = ImGui::GetIO();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

}

void App::Thread()
{
    while (true)
    {
        if(mWorker.completed)
        {
            mWorker.completed = false;
            mWorker.onCompleteCallback(PingAddress(),mWorker.completed);
        }
    }
}