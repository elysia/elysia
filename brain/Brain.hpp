#ifndef _ELYSIA_BRAIN_HPP_
#define _ELYSIA_BRAIN_HPP_
#include "ProteinDensity.hpp"
namespace Elysia {
class ProteinEnvironment;
class Neuron;
class Synapse; 
class SpatialSearch;

class Brain {
    ProteinEnvironment *mProteinMap;
    SpatialSearch *mSpatialSearch;
    ///creature maturity from 0(just born) to 1(adult)
    float mAge;
public:
    /**
     * Brain constructor
     * \param proteinMap the map of where proteins are being deployed in the brain so that neurons may grow appropriately. Brain takes ownership of pointer and destroys it
     */
    Brain (ProteinEnvironment *proteinMap);
    SimTime mCurTime;
    float getAge()const{return mAge;}
    std::list<Neuron *>mActiveNeurons;
    std::list<Synapse *>mActiveSynapses;
	std::set<Neuron *>mAllNeurons;
	std::set<Synapse *>mAllSynapses;

    void tick();
	void processSynapse();
	void processNeuron();
    ~Brain();
    std::list<Synapse *>::iterator activateSynapse(Synapse *activeSynapse);
    std::list<Neuron *>::iterator activateNeuron(Neuron *activeNeuron);
    void inactivateSynapse(Synapse *inactiveSynapse);
    void inactivateNeuron(Neuron *inactiveSynapse);
    std::list<Synapse *>::iterator activeSynapseListSentinel(){return mActiveSynapses.end();}
    std::list<Neuron *>::iterator activeNeuronListSentinel(){return mActiveNeurons.end();}
    ProteinEnvironment* getProteinEnvironment() {
        return mProteinMap;
    }
    SpatialSearch *getSpatialSearch(){
        return mSpatialSearch;
    }
};
}

#endif
