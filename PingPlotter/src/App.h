#pragma once
#include <condition_variable>
#include <functional>
#include <plf_nanotimer.h>
#include <SDL3/SDL_video.h>

#include "imgui.h"
#include "icmplib.h" // Must be above windows.h defined in file below

const int MAX_INTERVAL_MS = 100000;
const int MIN_INTERVAL_MS = 1;

const int MAX_DATAPOINTS = 100000;
const int INITIAL_DATA_TO_VIEW = 500;
class App
{
public:
	void SetColours();
	App(SDL_Window* window);
	~App();
	void Update();
	double PingAddress();
	void RenderAppUI();
	ImVec4 GetClearColour() { return clear_color; }
	bool mMinimised = false;

private:
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	void Thread();
	void SortData();
	void ClearVisualiser();
	void LoadWindowIcon();

	float* mCurrentTime;
	float* mPingTimes;
	int mThreadSleepTime = 5;
	int mMaxDataDisplay = 500;
	int mMaxDataUIInput = 500;
	float mCumulativePing = 0.0f;
	int mPingCount = 0;
	plf::nanotimer mAppTimer;
	bool mFirstRun = true;
	bool mPingsStarted = false;
	bool mShowAllData = true;
	int mIntervalBoxWidth = 40;
	SDL_Window* mSDLWindow;
	SDL_Surface* mIconSurface;
	ImVec4 mCustomBackgroundColour = { 0.19f, 0.19f, 0.18f, 0.9f };
	ImVec4 mCustomColour = { 0.95f, 0.6f, 0.2f, 0.8f };
	ImVec4 mCustomColourFull = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 1 };
	ImVec4 mCustomColourDim = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 0.6 };
	ImVec4 mCustomColourDimmer = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 0.4 };

};

struct WorkerThread
{
	std::thread thread;
	std::function<void(double,bool&)> onCompleteCallback;
	bool completed = false;
	bool running = true;
};


static WorkerThread mWorker;
