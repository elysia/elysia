#include <boost/thread.hpp>
namespace Elysia {
class GraphicsSystem {
    std::tr1::shared_ptr<boost::thread> mRenderThread;
public:
    int getWidth()const;
    int getHeight()const;
    GraphicsSystem();
    ~GraphicsSystem();
};
extern int glutKeyDown[256];
extern int glutSpecialKeyDown[256];

void Deinitialize();
}
