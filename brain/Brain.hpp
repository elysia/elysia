#ifndef _ELYSIA_BRAIN_HPP_
#define _ELYSIA_BRAIN_HPP_
#include "ProteinDensity.hpp"
namespace Elysia {

class Brain {
    ProteinDensity mDensity;
public:
    SimTime mCurTime;

    std::list<Neuron *>activeNeurons;
    std::list<Synapse *>activeSynapses;

    void tick();
};

}

#endif
