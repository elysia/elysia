#include <boost/thread.hpp>

namespace Elysia {
class BRAIN_CORE_EXPORT MainThread{public:
    static std::tr1::shared_ptr<boost::thread> giveUpMain(const std::tr1::function<void()>&continuation);
    static std::tr1::shared_ptr<boost::thread> wrestMainThread(const std::tr1::function<void()>&threadFunc);
};

}
