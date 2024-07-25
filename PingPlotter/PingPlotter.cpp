#include "PingPlotter.h"

#include "App.h"
#include "ImGuiProgressIndicators.h"
#include "implot.h"

PingPlotter::PingPlotter()
{
    mAppTimer.start();
    mCurrentTime = new float[MAX_DATAPOINTS];
    mPingTimes = new float[MAX_DATAPOINTS];
    for (int i = 0; i < MAX_DATAPOINTS; i++) { mCurrentTime[i] = 0; mPingTimes[i] = 0; }

    mWorker.thread = std::thread(&PingPlotter::Thread, this);
    mWorker.onCompleteCallback = [this](double result, bool& completed)
        {
            completed = true;
            mPingTimes[mPingCount] = result;
            mCurrentTime[mPingCount] = mAppTimer.get_elapsed_ms() / 1000.0f;

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

PingPlotter::~PingPlotter()
{
    mWorker.running = false;
    mWorker.thread.join();
    delete[] mCurrentTime;
    delete[] mPingTimes;
}

bool PingPlotter::Update()
{
    RenderAppUI();
    return 1;
}

void PingPlotter::RenderAppUI()
{
    // Make the background a DockSpace
    {
#ifdef _DEBUG
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar);
#else
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoResize);
#endif
    }

    if (!mPingsStarted)
    {
        {
            ImGui::Begin("Progress Indicator", nullptr, ImGuiWindowFlags_NoDecoration);

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
            ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
#endif
            if (ImGui::Checkbox("View All", &mShowAllData))
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

            if (ImGui::InputInt("Interval", &mThreadSleepTime, -1, -1))
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

                ImPlot::PushStyleColor(ImPlotCol_Line, { 0.95f, 0.6f, 0.2f, 0.8f });

                ImPlot::PlotLine("My Line Plot", currentTimeDisplay, pingTimesDisplay, dataDisplay);

                ImPlot::PopStyleColor();

                delete[] currentTimeDisplay;
                delete[] pingTimesDisplay;

                ImPlot::EndPlot();
            }
            
            ImGui::End();
        }

    }

}

void PingPlotter::Thread()
{
    while (mWorker.running)
    {
        if (mWorker.completed)
        {
            mWorker.completed = false;
            mWorker.onCompleteCallback(PingAddress(), mWorker.completed);
            std::this_thread::sleep_for(std::chrono::milliseconds(mThreadSleepTime));
        }
    }
}

void PingPlotter::ClearVisualiser()
{
    mPingCount = 0;
    mCumulativePing = 0;
}


double PingPlotter::PingAddress()
{
    icmplib::IPAddress ipAddress("8.8.8.8");
    return Ping(ipAddress, ICMPLIB_TIMEOUT_1S).delay;
}