#include "MultithreadingWorker.h"

#include <icmplib.h>

MultithreadingWorker::MultithreadingWorker(std::function<void (double, bool&)> callbackFunc)
{
    /*std::async;*/

    mWorker.thread = std::thread(&MultithreadingWorker::Thread, this);
    mWorker.onCompleteCallback = callbackFunc;
}

MultithreadingWorker::~MultithreadingWorker()
{
    mWorker.running = false;
    mWorker.thread.join();
}

void MultithreadingWorker::Thread()
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

double MultithreadingWorker::PingAddress()
{
    icmplib::IPAddress ipAddress("8.8.8.8");
    return Ping(ipAddress, ICMPLIB_TIMEOUT_1S).delay;
}