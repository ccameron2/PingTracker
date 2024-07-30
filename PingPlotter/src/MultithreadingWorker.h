#pragma once
#include <functional>
#include <thread>

class MultithreadingWorker
{
public:
	MultithreadingWorker(std::function<void(double,bool&)> callbackFunc);
	~MultithreadingWorker();

	void SetThreadSleepTime(int newTime) { mThreadSleepTime = newTime; }
	int* GetThreadSleepTime() { return &mThreadSleepTime; }
private:
	static void Thread();
	double PingAddress();

	int mThreadSleepTime = 5;

	struct WorkerThread
	{
		std::thread thread;
		std::function<void(double, bool&)> onCompleteCallback;
		bool completed = true;
		bool running = true;
	};
	static WorkerThread mWorker;
};

