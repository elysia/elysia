#include "Platform.hpp"
#include "MainThread.hpp"
namespace Elysia {
static boost::condition_variable gMainThreadCheckCondition;
static std::auto_ptr<boost::mutex> gMainThreadLock(new boost::mutex);
static bool gActiveMain=false;
static bool gActiveCallback=false;
std::tr1::shared_ptr<boost::thread> gFormerlyMainThread;
std::tr1::function<void()> gMainThreadFunc;
std::tr1::shared_ptr<boost::thread> MainThread::giveUpMain(const std::tr1::function<void()>&continuation){
    bool callme=true;
    std::tr1::function<void()> mainThreadFunc;    
    std::tr1::shared_ptr<boost::thread> y;
    {
        boost::unique_lock<boost::mutex> lok(*gMainThreadLock);
        gActiveMain=true;
        std::tr1::shared_ptr<boost::thread> x(new boost::thread(continuation));
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
std::tr1::shared_ptr<boost::thread> MainThread::wrestMainThread(const std::tr1::function<void()>&threadFunc){
    boost::unique_lock<boost::mutex> lok(*gMainThreadLock);
    if (gActiveMain) {
        gMainThreadFunc=threadFunc;
        gActiveCallback=true;
        gMainThreadCheckCondition.notify_one();
    }else {
#ifdef __APPLE__
        fprintf(stderr,"Warning: main thread not ceded to graphics\n");
#endif
        std::tr1::shared_ptr<boost::thread> x(new boost::thread(threadFunc));
        return x;
    }
    return std::tr1::shared_ptr<boost::thread>();
}


}
