#include "PingPlotter.h"

#include <fstream>

#ifdef WINDOWS
#include <shlobj_core.h>
#endif

#include "App.h"
#include "ImGuiProgressIndicators.h"
#include "implot.h"
#include "MultithreadingWorker.h"

PingPlotter::PingPlotter(AppColours& appColoursRef) : mAppColoursRef(appColoursRef)
{

    mAppTimer.start();

    mCurrentTime = new float[MAX_DATAPOINTS];
    mPingTimes = new float[MAX_DATAPOINTS];

    mPingDataDisplay = new float[mDataDisplaySize];
    mTimeDataDisplay = new float[mDataDisplaySize];

    for (int i = 0; i < MAX_DATAPOINTS; i++) { mCurrentTime[i] = 0; mPingTimes[i] = 0; }

    // Start multithreading worker and provide a callback
    mMultithreadingWorker = std::make_unique<MultithreadingWorker>([this](double result, bool& completed)
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
        });
}

PingPlotter::~PingPlotter()
{
    mMultithreadingWorker.reset();
    delete[] mCurrentTime;
    delete[] mPingTimes;
    delete[] mPingDataDisplay;
    delete[] mTimeDataDisplay;
}

bool PingPlotter::Update()
{
    float cumulativePing = 0;

    // Get segment of data to display from data arrays
    if (mPingCount < mMaxDataDisplaySize || mShowAllData)
    {
        mDataDisplaySize = mPingCount;
    }
    else
    {
        mDataDisplaySize = mMaxDataDisplaySize;
    }

    if (mDataDisplaySize != mPreviousDataDisplaySize)
    {
        delete[] mPingDataDisplay;
        delete[] mTimeDataDisplay;
        mPingDataDisplay = new float[mDataDisplaySize + 1]; // TODO Heap corruption fix?
        mTimeDataDisplay = new float[mDataDisplaySize + 1];
    }

    if (mPingCount >= mDataDisplaySize)
    {
        for (int i = 0; i < mDataDisplaySize; i++)
        {
            auto offset = mPingCount - mDataDisplaySize;
            mTimeDataDisplay[i] = mCurrentTime[i + offset];
            mPingDataDisplay[i] = mPingTimes[i + offset];

            cumulativePing += mPingDataDisplay[i];
            if (mPingDataDisplay[i] > mMaxPing) mMaxPing = mPingDataDisplay[i];
            if (mPingDataDisplay[i] < mMinPing) mMinPing = mPingDataDisplay[i];
        }
    }
    else
    {
        for (int i = 0; i < mDataDisplaySize; i++)
        {
            mTimeDataDisplay[i] = mCurrentTime[i];
            mPingDataDisplay[i] = mPingTimes[i];

            cumulativePing += mPingDataDisplay[i];
            if (mPingDataDisplay[i] > mMaxPing) mMaxPing = mPingDataDisplay[i];
            if (mPingDataDisplay[i] < mMinPing) mMinPing = mPingDataDisplay[i];
        }
    }
    mCumulativePing = cumulativePing;

    RenderAppUI();

    return true;
}

void PingPlotter::RenderAppUI()
{
    // Make the background a DockSpace
    {
#ifdef _DEBUG
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar);
#else
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoTabBar);
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
        {
            //static bool showDemoWindow = false;
            //ImGui::ShowDemoWindow(&showDemoWindow);
        }

        // Control panel
        if (mShowControlPanel)
        {
#ifdef _DEBUG
            ImGui::Begin("Controls");
#else
            ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
#endif
            if (ImGui::Checkbox("View All", &mShowAllData))
            {
                mMaxDataDisplaySize = mPingCount;
            }

            ImGui::SameLine();

            if (ImGui::Button("Clear"))
            {
                ClearVisualiser();
            }

            ImGui::SameLine();
            if (ImGui::Button("Output Data"))
            {
                OutputDataToCSV();
                ImGui::OpenPopup("Popup");
            }
            if (ImGui::BeginPopup("Popup"))
            {
                ImGui::Text("Saved to Documents/PingPlotter");
                ImGui::EndPopup();
            }

            ImGui::SameLine();

            ImGui::PushItemWidth(mIntervalBoxWidth);
            if (ImGui::InputInt("Interval to ping(ms)", &mThreadSleepTime, -1, -1))
            {
                if (mThreadSleepTime > MAX_INTERVAL_MS) mThreadSleepTime = MAX_INTERVAL_MS;
                if (mThreadSleepTime < MIN_INTERVAL_MS) mThreadSleepTime = MIN_INTERVAL_MS;
                mMultithreadingWorker->SetThreadSleepTime(mThreadSleepTime);
            }

            ImGui::SameLine();

            ImGui::Text("");

            ImGui::SameLine();
            ImGui::PushItemWidth(mColourPickerWidth);
            mAppColoursRef.RenderColourPicker();

            ImGui::SameLine();

        	// Max data limit slider
            if (mShowAllData)
            {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }

            ImGui::SameLine();
            ImGui::PushItemWidth(50);
            ImGui::DragInt("Max pings to display", &mMaxDataDisplaySize, 1, 5, mPingCount);
            if (mMaxDataDisplaySize <= 5) { mMaxDataDisplaySize = 5; }
            if (mMaxDataDisplaySize > mPingCount && mShowAllData == false) mMaxDataDisplaySize = mPingCount;

        	if (mShowAllData)
            {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
            //

            ImGui::SameLine();
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("Perf: %.3f ms (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::End();
        }

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

                ImPlot::PushStyleColor(ImPlotCol_Line, mAppColoursRef.GetColour());

                ImPlot::PlotLine("My Line Plot", mTimeDataDisplay, mPingDataDisplay, mDataDisplaySize);

                ImPlot::PopStyleColor();

                ImPlot::EndPlot();
            }
            ImGui::End();
        }

        // Show statistics
        {
            ImGui::Begin("Stats",nullptr,ImGuiWindowFlags_NoScrollbar);

            ImGui::Text("Current average ping: %.2fms", mCumulativePing / mDataDisplaySize);

        	ImGui::SameLine();
            ImGui::Text("| Max ping: %.2fms", mMaxPing);

            ImGui::SameLine();
            ImGui::Text("| Min ping: %.2fms", mMinPing);

            ImGui::SameLine();
            ImGui::Text("| Total time: %.2fs", mAppTimer.get_elapsed_ms() / 1000.0f);

        	ImGui::SameLine();
            ImGui::Checkbox("| Show Controls", &mShowControlPanel);

        	ImGui::SameLine();
#ifdef _DEBUG

#endif
            ImGui::End();
        }
    }

}


void PingPlotter::ClearVisualiser()
{
    mPingCount = 0.0f;
    mCumulativePing = 0.0f;
    mMaxPing = 0.0f;
    mMinPing = 999999.0f;
    mAppTimer.start();
}

void PingPlotter::OutputDataToCSV()
{
    std::string docsPath = "";
#ifdef WINDOWS

	PWSTR ppszPath;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &ppszPath);

	std::wstring wPath;
	if (SUCCEEDED(hr)) wPath = ppszPath;

	docsPath = std::string(wPath.begin(),wPath.end());
    docsPath += "\\PingPlotter\\";

#endif
    std::ofstream outFile(docsPath + "Output.csv", std::ios::trunc);
    for(int i = 0; i < mPingCount; i++)
    {
        outFile << mCurrentTime[i] << "," << mPingTimes[i] << "\n";
    }
    outFile.close();
}
