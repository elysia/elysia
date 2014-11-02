#include "Platform.hpp"
#include "MainThread.hpp"
namespace Elysia {
static std::condition_variable gMainThreadCheckCondition;
static std::auto_ptr<std::mutex> gMainThreadLock(new std::mutex);
static bool gActiveMain=false;
static bool gActiveCallback=false;
std::shared_ptr<std::thread> gFormerlyMainThread;
std::function<void()> gMainThreadFunc;
std::shared_ptr<std::thread> MainThread::giveUpMain(const std::function<void()>&continuation){
    bool callme=true;
    std::function<void()> mainThreadFunc;    
    std::shared_ptr<std::thread> y;
    {
        std::unique_lock<std::mutex> lok(*gMainThreadLock);
        gActiveMain=true;
        std::shared_ptr<std::thread> x(new std::thread(continuation));
        y=x;
        gFormerlyMainThread=x;
        gMainThreadCheckCondition.wait(lok);
        if (gActiveCallback) {
            gActiveCallback=false;
            mainThreadFunc=gMainThreadFunc;
        }
    }
    mainThreadFunc();
    return y;
}
std::shared_ptr<std::thread> MainThread::wrestMainThread(const std::function<void()>&threadFunc){
    std::unique_lock<std::mutex> lok(*gMainThreadLock);
    if (gActiveMain) {
        gMainThreadFunc=threadFunc;
        gActiveCallback=true;
        gMainThreadCheckCondition.notify_one();
    }else {
#ifdef __APPLE__
        fprintf(stderr,"Warning: main thread not ceded to graphics\n");
#endif
        std::shared_ptr<std::thread> x(new std::thread(threadFunc));
        return x;
    }
    return std::shared_ptr<std::thread>();
}


}
