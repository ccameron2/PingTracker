#pragma once
#include "icmplib.h" // Must be above windows.h

#include <functional>
#include <mutex>
#include <plf_nanotimer.h>
#include <thread>
#include <SDL3/SDL.h>

#include "PingTrackerSettings.h"

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
const int MAX_DATAPOINTS = 1000000;
const int MAX_WORKERS = 1; 

class PingTracker
{
public:
	PingTracker();
	~PingTracker();

	bool Update();
	void RenderAppUI();
	AppSettings* GetAppSettings() { return &mSettings; };

private:
	void ClearVisualiser();

	void OutputDataToCSV();

	PingTrackerSettings mSettings = PingTrackerSettings("PingTrackerSettings.ini");

	bool mShowControlPanel = false;
	
	double* mPingTimes;
	double* mDateTimes;
	double* mRawTimes;
	
	double* mPingDataDisplay;
	double* mDateTimeDataDisplay;
	double* mRawTimesDataDisplay;

	bool mUseDateTime = false;
	
	int mThreadSleepTime = 5;
	float mCumulativePing = 0.0f;
	
	double mMaxPing = 0.0;
	double mMinPing = 999999.0f;
	
	int mNumDataToDisplay = 0;

	std::mutex mMutex;
	
	int mPingCount = 0;
	plf::nanotimer mAppTimer;
	bool mPingsStarted = false;
	float mIntervalBoxWidth = 30.0f;
	float mColourPickerWidth = 80.0f;
	std::unique_ptr<MultithreadingWorker> mMultithreadingWorkers[MAX_WORKERS];
};