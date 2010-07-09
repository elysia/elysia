#include "BrainPlugin.hpp"
namespace Elysia {
class Brain;
class GraphicsSystem;
class Visualization:public BrainPlugin  {
    std::tr1::shared_ptr<GraphicsSystem> mGraphics;
    static std::tr1::weak_ptr<GraphicsSystem> mGlobalGraphics;
public:
    Visualization();
    void draw();
    void update();
    void initialize(Brain*b);
    ~Visualization();
};
BrainPlugin* makeVisualization(Brain*b);
}
