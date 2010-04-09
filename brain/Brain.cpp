#include "Platform.hpp"
#include "genome.pb.h"
#include "Brain.hpp"
#include "ProteinEnvironment.hpp"
#include "Neuron.hpp"
#include "Synapse.hpp"
#include "SimpleSpatialSearch.hpp"
namespace Elysia {

Brain::Brain (ProteinEnvironment *proteinMap){
    mProteinMap=proteinMap;
    mSpatialSearch=new SimpleSpatialSearch;
}

void Brain::tick(){
	processNeuron();
	processSynapse();
    ++mCurTime;
}


void Brain::processNeuron(){
	for (std::list<Neuron*>::iterator i=mActiveNeurons.begin(),ie=mActiveNeurons.end();
         i!=ie;
		 ++i) {
        (*i)->tick();
	mActiveNeurons.clear();
	}
}

void Brain::processSynapse(){
	for (std::list<Synapse*>::iterator i=mActiveSynapses.begin(),ie=mActiveSynapses.end();
         i!=ie;
		 ++i) {
        (*i)->residualFire();
	}
}

void Brain::inactivateSynapse(Synapse *inactiveSynapse){
	mActiveSynapses.erase(inactiveSynapse->mWhere);
	inactiveSynapse->mWhere= activeSynapseListSentinel();
}
void Brain::inactivateNeuron(Neuron *inactiveNeuron){
	mActiveNeurons.erase(inactiveNeuron->mWhere);
	inactiveNeuron->mWhere= activeNeuronListSentinel();
}

std::list<Synapse *>::iterator Brain::activateSynapse(Synapse *activeSynapse){
    mActiveSynapses.push_front(activeSynapse);
    return mActiveSynapses.begin();
}
std::list<Neuron *>::iterator Brain::activateNeuron(Neuron *activeNeuron){
  mActiveNeurons.push_back(activeNeuron);
  return mActiveNeurons.begin();
}


Brain::~Brain() {
    delete mSpatialSearch;
    delete mProteinMap;
}
}
