#include "Brain.hpp"
namespace Elysia {

void Brain::tick(){
   mCurTime++;
}

Brain::Brain(){
    mCurTime = 0;
}

}