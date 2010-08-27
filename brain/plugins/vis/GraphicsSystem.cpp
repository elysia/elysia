#include <Platform.hpp>
#include "GraphicsSystem.hpp"
#ifdef __APPLE__
#include "glut.h"
#else
#include "GL/glut.h"
//#include <GL/freeglut.h>
#endif
#include "Visualization.hpp"
int GAngle=0;
volatile bool gKillGraphics=false;
volatile bool gShutDown=false;
volatile bool gInvalidDisplayFunction=false;
volatile bool gShutdownComplete=false;
int gGlutWindowId=0;
std::tr1::shared_ptr<boost::thread> gRenderThread;
namespace Elysia {
std::vector<Visualization*> gToRender;
std::auto_ptr<boost::mutex>gRenderLock (new boost::mutex);
boost::condition_variable gRenderCompleteCondition;
boost::condition_variable gRenderCondition;


void Deinitialize() {
        {
            boost::unique_lock<boost::mutex> renderLock(*Elysia::gRenderLock);    
            Elysia::gToRender.clear();
            Elysia::gRenderCondition.notify_one();
            Elysia::gRenderCompleteCondition.notify_one();
        }
        gShutDown=true;
        gKillGraphics=false;
        fflush(stdout);
        fflush(stderr);
//        while(!gShutdownComplete);
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);//need to sleep to allow the gl context to destroy itself (mac requires the main thread exit by itself after the window is destroyed)
#endif
}
}
void GlutNop(){}

namespace Elysia {
static int gDisplayWidth=1024;
static int gDisplayHeight=768;
void Reshape(int w, int h) {
    gDisplayWidth=w;
    gDisplayHeight=h;
}

int GraphicsSystem::getWidth()const{
    return gDisplayWidth;

}
int GraphicsSystem::getHeight()const{
    return gDisplayHeight;
}
void Display(void) {
    if (!gKillGraphics) {
        glViewport(0,0,gDisplayWidth,gDisplayHeight);
        glClear(GL_COLOR_BUFFER_BIT);
        {
            boost::unique_lock<boost::mutex> lok(*gRenderLock);
            gRenderCompleteCondition.notify_one();
            if (gToRender.size()) {
                gRenderCondition.wait(lok);
                int heightPartition=1;
                if (gToRender.size()>3) {
                    heightPartition=2;
                }
                if (gToRender.size()>10) {
                    heightPartition=3;
                }
                int i=0;
                for (int h=0;h<heightPartition;++h) {
                    int wlim=gToRender.size()/heightPartition;
                    if (gToRender.size()%heightPartition) wlim++;
                    for (int w=0;w<wlim;++w,++i) { 
                        glViewport(w*gDisplayWidth/wlim,
                          h*gDisplayHeight/heightPartition,
                          (w+1)*gDisplayWidth/wlim,
                          (h+1)*gDisplayHeight/heightPartition);
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();
                        glScalef(2*wlim/(float)(gDisplayWidth),2*heightPartition/(float)(gDisplayHeight),1.0);
                        if (i<(int)gToRender.size())
                            gToRender[i]->draw();
                    }
                }
            }
        }
        {
            boost::unique_lock<boost::mutex> lok(*gRenderLock);
            gRenderCompleteCondition.notify_one();
        }
        glFlush();
    }else {
    }
    glutSwapBuffers();
    if (gShutDown ) {
        gInvalidDisplayFunction=true;
        glutDestroyWindow(gGlutWindowId);
    }
}
int glutKeyDown[256]={0};
int glutSpecialKeyDown[256]={0};
}
void Timer(int extra)
	{
        if (!gInvalidDisplayFunction) {
            glutPostRedisplay();
            glutTimerFunc(30,Timer,0);
        }else {
            gShutdownComplete=true;
        }
	}

void Idly() {
    if (!gInvalidDisplayFunction) {
        glutPostRedisplay();
    }
}
void kbd(unsigned char key, int x, int y) {
    Elysia::glutKeyDown[key]=1;
}
void kbdUp(unsigned char key, int x, int y) {
    Elysia::glutKeyDown[key]=0;
}
volatile bool myfuncInitialized=false;
void myfunc() {
        // Enable Front Face
        //glEnable(GL_CULL_FACE);
    const char *argv[1];
    argv[0]="elysia";
    int argc=1;
    static bool firstTime=true;
    {
        boost::unique_lock<boost::mutex> lok(*Elysia::gRenderLock);
        if (gKillGraphics ){
            gKillGraphics=false;
        }else {
            
        }
        if (firstTime)
            glutInit(&argc, (char**)argv);
        firstTime=false;
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
        glutInitWindowSize(Elysia::gDisplayWidth,Elysia::gDisplayHeight);

        gGlutWindowId=glutCreateWindow("This is the window title");
        //glutIdleFunc(Idly);
        glutDisplayFunc(Elysia::Display);
        glutReshapeFunc(Elysia::Reshape);
        glutKeyboardFunc(&kbd);
        glutKeyboardUpFunc(&kbdUp);
        glutTimerFunc(0,Timer,0);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        Elysia::gRenderCondition.notify_one();
    }
    myfuncInitialized=true;
	glutMainLoop();

    for (int i=0;i<10;++i) {
        printf("Testing thread: %d/10\n",i+1);
    }
    gKillGraphics=false;
}
namespace Elysia {

GraphicsSystem::GraphicsSystem () {
    if (gRenderThread) {
        mRenderThread=gRenderThread;
        gKillGraphics=false;
    }else {
        boost::unique_lock<boost::mutex> renderLock(*Elysia::gRenderLock);    
        
        std::tr1::shared_ptr<boost::thread> x(new boost::thread(&myfunc));
        gRenderCondition.wait(renderLock);        
        mRenderThread=gRenderThread=x;
        gKillGraphics=false;

    }
}
GraphicsSystem::~GraphicsSystem () {
    gKillGraphics=true;
          
}
}
