#include "../../Platform.hpp"
#include "Visualization.hpp"
#include "GraphicsSystem.hpp"
namespace Elysia {
std::tr1::weak_ptr<GraphicsSystem> Visualization::mGlobalGraphics;
BrainPlugin* makeVisualization(Brain*b) {
    BrainPlugin*v=new Visualization;
    v->initialize(b);
    return v;
}
Visualization::Visualization(){
}
void Visualization::update() {
}
void Visualization::initialize( Brain*b) {
    mGraphics=mGlobalGraphics.lock();
    if (!mGraphics) {
        std::tr1::shared_ptr<GraphicsSystem> tmp(new GraphicsSystem());
        mGraphics = tmp;
        mGlobalGraphics=tmp;
    }
    

}
Visualization::~Visualization() {
    mGraphics=std::tr1::shared_ptr<GraphicsSystem>();
}
}
