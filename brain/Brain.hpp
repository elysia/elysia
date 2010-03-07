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
    ~Brain();
    std::list<Synapse *>::iterator activeSynapse(Synapse *activeSynapse);
    std::list<Neuron *>::iterator activeNeuron(Neuron *activeNeuron);
    std::list<Synapse *>::iterator inactiveSynapse(){return mActiveSynapses.end();}
    std::list<Neuron *>::iterator inactiveNeuron(){return mActiveNeurons.end();}
};
}

#endif
