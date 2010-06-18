#include <Platform.hpp>
#include "GraphicsSystem.hpp"

void myfunc() {
    for (int i=0;i<10;++i) {
        printf("Testing thread: %d/10\n",i+1);
    }
}
namespace Elysia {

GraphicsSystem::GraphicsSystem ():mRenderThread(&myfunc) {
    
}
GraphicsSystem::~GraphicsSystem () {

}
}
