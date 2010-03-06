#include "Platform.hpp"
#include "genome.pb.h"
#include "Brain.hpp"
#include "ProteinEnvironment.hpp"
namespace Elysia {
Brain::Brain (ProteinEnvironment *proteinMap){
    mProteinMap=proteinMap;
}
void Brain::tick(){
    ++mCurTime;
}

Brain::~Brain() {
    delete mProteinMap;
}
}
