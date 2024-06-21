#pragma once
#include <condition_variable>
#include <functional>

#include "imgui.h"
#include "icmplib.h" // Must be above windows.h defined in file below
#include "Timer.h"

const int MAX_DATAPOINTS = 100;

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

	float mCurrentTime[MAX_DATAPOINTS];
	float mPingTimes[MAX_DATAPOINTS];

	int pingCount = 0;
	Timer appTimer;
	bool first = true;
};

struct WorkerThread
{
	std::thread             thread;
	std::condition_variable workReady;
	std::mutex              lock;
	std::function<void(double,bool&)> onCompleteCallback;
	bool completed = false;
};


static WorkerThread mWorker;
