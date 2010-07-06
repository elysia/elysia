#include <Platform.hpp>
#include "GraphicsSystem.hpp"
#ifdef __APPLE__
#include "glut.h"
#else
#include "GL/glut.h"
//#include <GL/freeglut.h>
#endif
int GAngle=0;
volatile bool gKillGraphics=false;
volatile bool gShutDown=false;
volatile bool gInvalidDisplayFunction=false;
volatile bool gShutdownComplete=false;
int gGlutWindowId=0;
std::tr1::shared_ptr<boost::thread> gRenderThread;
void Deinitialize() {
    if (gKillGraphics) {
        gShutDown=true;
        gKillGraphics=false;
        fflush(stdout);
        fflush(stderr);
        while(!gShutdownComplete);
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);//need to sleep to allow the gl context to destroy itself (mac requires the main thread exit by itself after the window is destroyed)
#endif
    }
}
void GlutNop(){}
void Display(void) {
    if (!gKillGraphics) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glLoadIdentity();
        glRotated(GAngle,0,1,0);
        
        // Anti-Clockwise Winding
        glBegin(GL_TRIANGLES);
		glVertex3f(-1,0,0);
		glVertex3f(1,0,0);
		glVertex3f(0,1,0);
        glEnd();
        
        GAngle = GAngle + 4;
        
        glFlush();
    }else {
    }
    glutSwapBuffers();
    if (gShutDown ) {
        gInvalidDisplayFunction=true;
        glutDestroyWindow(gGlutWindowId);
    }
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

void myfunc() {
    const char *argv[1];
    argv[0]="elysia";
    int argc=1;
    if (gKillGraphics ){
        gKillGraphics=false;
    }else {
        glutInit(&argc, (char**)argv);
        
    }
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    //glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	gGlutWindowId=glutCreateWindow("This is the window title");
	glutDisplayFunc(Display);
	glutTimerFunc(0,Timer,0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1,1,-1,1,1,3);
	glTranslated(0,0,-2);
	glMatrixMode(GL_MODELVIEW);
    
	// Enable Front Face
	glEnable(GL_CULL_FACE);
    atexit(Deinitialize);
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
        std::tr1::shared_ptr<boost::thread> x(new boost::thread(&myfunc));
        mRenderThread=gRenderThread=x;
        gKillGraphics=false;

    }
}
GraphicsSystem::~GraphicsSystem () {
    gKillGraphics=true;
}
}
