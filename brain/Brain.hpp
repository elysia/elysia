#ifndef _ELYSIA_BRAIN_HPP_
#define _ELYSIA_BRAIN_HPP_
#include "ProteinDensity.hpp"
namespace Elysia {
class ProteinEnvironment;
class Neuron;
class Synapse; 

class Brain {
    ProteinEnvironment *mProteinMap;
public:
    /**
     * Brain constructor
     * \param proteinMap the map of where proteins are being deployed in the brain so that neurons may grow appropriately. Brain takes ownership of pointer and destroys it
     */
    Brain (ProteinEnvironment *proteinMap);
    SimTime mCurTime;

    std::list<Neuron *>mActiveNeurons;
    std::list<Synapse *>mActiveSynapses;

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
};
}

#endif
