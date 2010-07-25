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

/**
 *  @param ProteinEnvironment *proteinMap - a base protein map for the brain
 *
 *	Description:	Creates a new Brain instance using a defined protein map as a base.
 *					Also adds a new spatial search instance, sets the brain's age to 0 and
 *					initializes all requisite brain plugins
**/
Brain::Brain (ProteinEnvironment *proteinMap){
    mProteinMap=proteinMap;
    mSpatialSearch=new SimpleSpatialSearch;
    mAge=0;
    BrainPlugins::constructAll(this).swap(mPlugins);
}

/**
 *	Description:	One "tick" of the brain simulation code, where the following occurs:
 *					 - processNeuron() called
 *					 - processSynapes() called
 *					 - increments the current time in the simulation
 *					 - ages the brain
 *					 - updates all the plugins for this brain
**/
void Brain::tick(){
	processNeuron();
	processSynapse();
    ++mCurTime;
    mAge+=1.0e-6;//fixme this is probably not correct: we probably need genes to do this
    if (mAge>1.0) mAge=1.0;
	/*
    for(std::vector<BrainPlugin*>::iterator i=mPlugins.begin(),ie=mPlugins.end();i!=ie;++i) {
        (*i)->update();
    }
	*/
}


/**
 *	Description:	Updates all neurons in the neuron list for this brain, then clears the list of
 *					active neurons
**/
void Brain::processNeuron(){
	for (std::list<Neuron*>::iterator i=mActiveNeurons.begin(),ie=mActiveNeurons.end();
         i!=ie;
		 ++i) {
        (*i)->tick();
	}
	mActiveNeurons.clear();
}

/**
 *	Description:	Iterates through brain's associates synapses and updates all of them using
 *					the Synapse class residualFire() method
**/
void Brain::processSynapse(){
    std::list<Synapse*>::iterator j,i=mActiveSynapses.begin(),ie=mActiveSynapses.end();
	while (i!=ie) {
        std::list<Synapse*>::iterator j=i++;
        (*j)->residualFire();
	}
}

/**
 *	@param Synapse *inactiveSynapse - synapse to remove
 *
 *	Description:	Removes a synapse from the synapse list and updates it.
**/
void Brain::inactivateSynapse(Synapse *inactiveSynapse){
	mActiveSynapses.erase(inactiveSynapse->mWhere);
	inactiveSynapse->mWhere=activeSynapseListSentinel();
}

/**
 *	@param Neuron *inactiveNeuron - an inactive neuron to remove
 *
 *	Description:	Removes an unused neuron from the neuron list and updates
**/
void Brain::inactivateNeuron(Neuron *inactiveNeuron){
	mActiveNeurons.erase(inactiveNeuron->mWhere);
	inactiveNeuron->mWhere=activeNeuronListSentinel();
}

/**
 *	@param Synapse *activeSynapse - a synapse to activate
 *	@return	the beginning element of the active synapses list for this brain
 *
 *	Description:	Adds a new synapse to the brain (at front of active synapse list)
 *					and returns the front element of the list
**/
std::list<Synapse *>::iterator Brain::activateSynapse(Synapse *activeSynapse){
    mActiveSynapses.push_front(activeSynapse);
    return mActiveSynapses.begin();
}
/**
 *	@param Neuron *activeNeuron - a neuron to activate
 *	@return	the beginning element of the active neuron list for this brain
 *
 *	Description:	Adds a new neuron to the brain (at front of active neuron list)
 *					and returns the front element of the list
**/
std::list<Neuron *>::iterator Brain::activateNeuron(Neuron *activeNeuron){
  mActiveNeurons.push_front(activeNeuron);
  return mActiveNeurons.begin();
}

/**
 *	Brain destructor
**/
Brain::~Brain() {
	for(std::set<Neuron*>::iterator i=mAllNeurons.begin(),ie=mAllNeurons.end();i!=ie;++i){
		delete *i;
	}
    delete mSpatialSearch;
    delete mProteinMap;
    for (std::vector<BrainPlugin*>::iterator i=mPlugins.begin(),ie=mPlugins.end();i!=ie;++i) {
        delete *i;
    }
}
/**
 * @return the bounds of all genes in the brain, i.e. where the farthest neurons may spout
 */
BoundingBox3f3f Brain::getBounds()const{
    
    
//    return BoundingBox3f3f(Vector3f(0,00,00),Vector3f(1000,1000,1000));
    return mProteinMap->getBounds();
}

}

