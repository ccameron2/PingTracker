
#include "App.h"


#include <iostream>
#include <string>


#include "implot.h"

void App::Update()
{
    mPingTimes[frameCount] = PingAddress();

    mCurrentTime[frameCount] = appTimer.GetTime();

    if(frameCount + 1 < MAX_DATAPOINTS)
    {
        frameCount++;
    }
    else
    {
        frameCount = 0;
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

