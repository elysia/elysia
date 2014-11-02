#include <thread>

namespace Elysia {
class BRAIN_CORE_EXPORT MainThread{public:
    static std::shared_ptr<std::thread> giveUpMain(const std::function<void()>&continuation);
    static std::shared_ptr<std::thread> wrestMainThread(const std::function<void()>&threadFunc);
};

}
