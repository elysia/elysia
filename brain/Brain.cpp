#include "Platform.hpp"
#include "genome.pb.h"
#include "Brain.hpp"
#include "ProteinEnvironment.hpp"
#include "Neuron.hpp"
#include "Synapse.hpp"
#include "SimpleSpatialSearch.hpp"
#include "BrainPlugins.hpp"
#include "BrainPlugin.hpp"
namespace Elysia {

Brain::Brain (ProteinEnvironment *proteinMap){
    mProteinMap=proteinMap;
    mSpatialSearch=new SimpleSpatialSearch;
    mAge=0;
    BrainPlugins::constructAll(this).swap(mPlugins);
}

void Brain::tick(){
	processNeuron();
	processSynapse();
    ++mCurTime;
    mAge+=1.0e-6;//fixme this is probably not correct: we probably need genes to do this
    if (mAge>1.0) mAge=1.0;
    for(std::vector<BrainPlugin*>::iterator i=mPlugins.begin(),ie=mPlugins.end();i!=ie;++i) {
        (*i)->update();
    }
}


void Brain::processNeuron(){
	for (std::list<Neuron*>::iterator i=mActiveNeurons.begin(),ie=mActiveNeurons.end();
         i!=ie;
		 ++i) {
        (*i)->tick();
	}
	mActiveNeurons.clear();
}

void Brain::processSynapse(){
    std::list<Synapse*>::iterator j,i=mActiveSynapses.begin(),ie=mActiveSynapses.end();
	while (i!=ie) {
        std::list<Synapse*>::iterator j=i++;
        (*j)->residualFire();
	}
}

void Brain::inactivateSynapse(Synapse *inactiveSynapse){
	mActiveSynapses.erase(inactiveSynapse->mWhere);
	inactiveSynapse->mWhere=activeSynapseListSentinel();
}
void Brain::inactivateNeuron(Neuron *inactiveNeuron){
	mActiveNeurons.erase(inactiveNeuron->mWhere);
	inactiveNeuron->mWhere=activeNeuronListSentinel();
}

std::list<Synapse *>::iterator Brain::activateSynapse(Synapse *activeSynapse){
    mActiveSynapses.push_front(activeSynapse);
    return mActiveSynapses.begin();
}
std::list<Neuron *>::iterator Brain::activateNeuron(Neuron *activeNeuron){
  mActiveNeurons.push_front(activeNeuron);
  return mActiveNeurons.begin();
}


Brain::~Brain() {
    delete mSpatialSearch;
    delete mProteinMap;
}
}
