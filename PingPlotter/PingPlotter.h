#pragma once
#include "icmplib.h" // Must be above windows.h

#include <functional>
#include <plf_nanotimer.h>
#include <thread>
#include <SDL3/SDL.h>

struct WorkerThread
{
	std::thread thread;
	std::function<void(double, bool&)> onCompleteCallback;
	bool completed = true;
	bool running = true;
};
static WorkerThread mWorker;

const int MAX_INTERVAL_MS = 100000;
const int MIN_INTERVAL_MS = 1;

const int MAX_DATAPOINTS = 100000;
const int INITIAL_DATA_TO_VIEW = 500;

class PingPlotter
{
public:
	PingPlotter();
	~PingPlotter();

	bool Update();
	void RenderAppUI();

private:
	void Thread();
	void ClearVisualiser();
	double PingAddress();

	float* mCurrentTime;
	float* mPingTimes;
	int mThreadSleepTime = 5;
	int mMaxDataDisplay = 500;
	int mMaxDataUIInput = 500;
	float mCumulativePing = 0.0f;
	int mPingCount = 0;
	plf::nanotimer mAppTimer;
	bool mPingsStarted = false;
	bool mShowAllData = true;
	int mIntervalBoxWidth = 40;
	SDL_Window* mSDLWindow;
};

