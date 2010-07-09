#include "../../Platform.hpp"
#include "Visualization.hpp"
#include "GraphicsSystem.hpp"
#ifdef __APPLE__
#include "glut.h"
#else
#include "GL/glut.h"
//#include <GL/freeglut.h>
#endif
namespace Elysia {
extern boost::mutex gRenderLock;
extern boost::condition_variable gRenderCondition;
extern boost::condition_variable gRenderCompleteCondition;

extern std::vector<Visualization*> gToRender;
std::tr1::weak_ptr<GraphicsSystem> Visualization::mGlobalGraphics;
BrainPlugin* makeVisualization(Brain*b) {
    BrainPlugin*v=new Visualization;
    v->initialize(b);
    return v;
}
Visualization::Visualization(){
    boost::unique_lock<boost::mutex> renderLock(gRenderLock);
    if (std::find(gToRender.begin(),gToRender.end(),this)==gToRender.end()) {
        gToRender.push_back(this);
    }
}
void Visualization::update() {
    {
        boost::unique_lock<boost::mutex> renderLock(gRenderLock);    
        gRenderCondition.notify_one();
    }

    {
        boost::unique_lock<boost::mutex> renderLock(gRenderLock);    
        gRenderCompleteCondition.wait(renderLock);
    }
}
void Visualization::initialize( Brain*b) {
    mGraphics=mGlobalGraphics.lock();
    if (!mGraphics) {
        std::tr1::shared_ptr<GraphicsSystem> tmp(new GraphicsSystem());
        mGraphics = tmp;
        mGlobalGraphics=tmp;
    }
    

}
void Visualization::draw() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Anti-Clockwise Winding
    glBegin(GL_TRIANGLES);
    glVertex3f(0,0,.5);
    glVertex3f(1,0,.5);
    glVertex3f(0,1,.5);
    glVertex3f(0,0,-.5);
    glVertex3f(1,0,-.5);
    glVertex3f(0,1,-.5);


    glVertex3f(0,0,-.5);
    glVertex3f(0,1,-.5);
    glVertex3f(1,0,-.5);
    glVertex3f(0,0,.5);
    glVertex3f(0,1,.5);
    glVertex3f(1,0,.5);
    glEnd();
    
}
Visualization::~Visualization() {
    {
        boost::unique_lock<boost::mutex> renderLock(gRenderLock);
        std::vector<Visualization*>::iterator where=std::find(gToRender.begin(),gToRender.end(),this);
        if (where!=gToRender.end()) {
            gToRender.erase(where);
        }
        
    }
    mGraphics=std::tr1::shared_ptr<GraphicsSystem>();

}
}
