#ifndef _ELYSIA_SYNAPSE_HPP_
#define _ELYSIA_SYNAPSE_HPP_


namespace Elysia {
class CellComponent;
class Neuron;
class Branch;
class ActivityStats;
class Brain;
///The dendrite_tip represents connections from the dendrite tree onto other neurons. Neurons pass activity onto
///dendrite_tips and dendrite_tips pass that activity onto branches

class BRAIN_CORE_EXPORT Synapse{
    Brain *mBrain;
	std::list<Synapse*>::iterator mWhere;
    friend class Brain;
    Neuron *mRecipientNeuron;
public:
	~Synapse();
    float mSignalWeight;
    int mFiringWindow;		//How long this synapse stays active for
    int mFiringCounter;     //Starts at mFiringWindow and counts down to 0 (then synapse deactivates)
    int mDevelopmentStage;	//The synapse tracks whether it is in a stage of early excitation

	CellComponent* mParentBranch;

    ///float used in development only and reset when connect is called to unit (1.0) <standard in development>
    float mConnectionStrength;
    bool detach();		//Detach dendrite tip from target neuron return 1 for success
    void connect();     //need to get the location bound from neuron
	Synapse(CellComponent * parent);
	void fireSynapse();
	void residualFire();
	void develop(const ActivityStats& stats);
	void visualizeSynapse(FILE *dendriteTree, size_t parent);
	void setDevelopmentStage(int stage);
	int getDevelopmentStage();
    Neuron *recipient() const{
        return mRecipientNeuron;
    }
};

}
#endif
