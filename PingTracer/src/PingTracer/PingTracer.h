#pragma once
#include "icmplib.h" // Must be above windows.h

#include <functional>
#include <plf_nanotimer.h>
#include <thread>
#include <SDL3/SDL.h>

#include "PingTracerSettings.h"

#include "App/AppColours.h"
#include "MultithreadingWorker.h"

struct WorkerThread
{
	std::thread thread;
	std::function<void(double, bool&)> onCompleteCallback;
	bool completed = true;
	bool running = true;
};
static WorkerThread mWorker;

const int MAX_INTERVAL_MS = 1000;
const int MIN_INTERVAL_MS = 1;

const int MAX_DATAPOINTS = 100000;

class PingTracer
{
public:
	PingTracer();
	~PingTracer();

	bool Update();
	void RenderAppUI();
	AppSettings* GetAppSettings() { return &mSettings; };

private:
	void ClearVisualiser();

	void OutputDataToCSV();

	PingTracerSettings mSettings = PingTracerSettings("PingTracerSettings.ini");

	bool mShowControlPanel = false;
	float* mCurrentTime;
	float* mPingTimes;
	int mThreadSleepTime = 5;
	float mCumulativePing = 0.0f;
	float mMaxPing = 0.0f;
	float mMinPing = 999999.0f;
	int mNumDataToDisplay = 0;

	float* mPingDataDisplay;
	float* mTimeDataDisplay;

	int mPingCount = 0;
	plf::nanotimer mAppTimer;
	bool mPingsStarted = false;
	float mIntervalBoxWidth = 30.0f;
	float mColourPickerWidth = 80.0f;
	std::unique_ptr<MultithreadingWorker> mMultithreadingWorker;
};