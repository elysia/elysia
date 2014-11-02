#include <thread>
namespace Elysia {
class GraphicsSystem {
    std::shared_ptr<std::thread> mRenderThread;
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
