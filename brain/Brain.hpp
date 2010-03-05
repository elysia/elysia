#ifndef _ELYSIA_BRAIN_HPP_
#define _ELYSIA_BRAIN_HPP_
#include "ProteinDensity.hpp"
namespace Elysia {
class Brain {
    ProteinDensity mDensity;
public:
    SimTime mCurTime;

    std::list<Neuron *>mActiveNeurons;
    std::list<Synapse *>mActiveSynapses;

    void tick();
};

}

#endif
