#include <boost/thread.hpp>
namespace Elysia {
class GraphicsSystem {
    boost::thread mRenderThread;
public:
    GraphicsSystem();
    ~GraphicsSystem();
};
}
