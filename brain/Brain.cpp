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

std::list<Synapse *>::iterator Brain::activeSynapse(Synapse *activeSynapse){
    mActiveSynapses.push_front(activeSynapse);
    return mActiveSynapses.begin();
}
std::list<Neuron *>::iterator Brain::activeNeuron(Neuron *activeNeuron){
  mActiveNeurons.push_back(activeNeuron);
  return mActiveNeurons.begin();
}

Brain::~Brain() {
    delete mProteinMap;
}
}
