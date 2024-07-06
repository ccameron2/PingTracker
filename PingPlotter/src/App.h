#pragma once
#include <condition_variable>
#include <functional>
#include <plf_nanotimer.h>

#include "imgui.h"
#include "icmplib.h" // Must be above windows.h defined in file below

const int MAX_INTERVAL_MS = 100000;
const int MIN_INTERVAL_MS = 1;

const int MAX_DATAPOINTS = 100000;
const int INITIAL_DATA_TO_VIEW = 500;
class App
{
public:
	App();
	~App();
	void Update();
	double PingAddress();
	void RenderAppUI();
	ImVec4 GetClearColour() { return clear_color; }
private:
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	void Thread();
	void SortData();
	void ClearVisualiser();
	void LoadWindowIcon(std::string fileName);
	float* mCurrentTime;
	float* mPingTimes;
	int mThreadSleepTime = 10;
	int mMaxDataDisplay = 500;

	int mPingCount = 0;
	plf::nanotimer mAppTimer;
	bool mFirstRun = true;
	bool mPingsStarted = false;
	bool mShowAllData = false;
	bool mDarkMode = true;
	int mIntervalBoxWidth = 40;
};

struct WorkerThread
{
	std::thread thread;
	std::function<void(double,bool&)> onCompleteCallback;
	bool completed = false;
	bool running = true;
};


static WorkerThread mWorker;
