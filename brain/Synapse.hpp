#ifndef _ELYSIA_SYNAPSE_HPP_
#define _ELYSIA_SYNAPSE_HPP_

namespace Elysia {
class CellComponent;
class Neuron;
class Branch;
///The dendrite_tip represents connections from the dendrite tree onto other neurons. Neurons pass activity onto
///dendrite_tips and dendrite_tips pass that activity onto branches

class Synapse{
public:
    float mSignalWeight;
    int mFiringWindow;		//How long this synapse stays active for
    int mFiringCounter;     //Starts at mFiringWindow and counts down to 0 (then synapse deactivates)
    CellComponent* mParentBranch;
    Neuron *mRecipientNeuron;
    float mConnectionStrength;
    //Detach dendrite tip from target neuron return 1 for success
    bool detach();
    void connect();     //need to get the location bound from neuron
	Synapse(CellComponent * parent);
	void fireSynapse(float signal);
	void developSynapse();
};

}
#endif
