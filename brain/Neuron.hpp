#ifndef _ELYSIA_NEURON_HPP_
#define _ELYSIA_NEURON_HPP_
#include "CellComponent.hpp"
#include "ActivityStats.hpp"
#include <vector>

namespace Elysia {
namespace Genome {
class Gene;
}

class Synapse;
class ActivityStats;
/**
 * The neuron class receives activity from the dendrite class and then passes activity onto the dendrite tips that connect to it
 */
class Neuron : public CellComponent, ActivityStats{
    ProteinDensity *mProteinDensity;
    std::list<Neuron*>::iterator mWhere;
    Brain*mBrain;
    friend class Brain;
private:
    ///Neuron location
    Vector3f mNeuronLocation;
    /////WE NO LONGER USE LOBES AT LEAST FOR NOWLobe *parent_lobe;
    //Array of dendrites connections
    std::vector<Synapse* >mConnectedSynapses;
    /**
     * Fire activates a DendriteTip
     * \param target is the DendriteTip that should be activated
     * \returns whether the signal successfully passed through the inter-neuron gap
     */
    float BaseBranchiness;
    float TipBranchiness;
    float TreeDepth;
    float BaseThreshold;
    float TipThreshold;
    float FiringTime;			//Holding signal could be done solely on the synapse. Presently unused
    float ReceptivityTime;
    float LearningResponsiveness;
    float InhibitionSignal;
    float mRandomDepthDeterminer;
    float mRandomBranchDeterminer;
	float mNeuronSignalWeight;
    SimTime mLastActivity;
	int mDevelopmentCounter;			//Counts down to 0 to trigger next developmental re-evaluation
	

    virtual Neuron*getParentNeuron(){return this;}

public:
    Neuron(Brain * brain, float BaseBranchiness, float TipBranchiness, float TreeDepth, const Vector3f &location, const Elysia::Genome::Gene&spawningGene);
    ~Neuron();
    void fire();
    ///Simulates one millisecond of neural time
    void tick();
    const Vector3f& getLocation()const {return mNeuronLocation;}
	ProteinDensity& getProteinDensityStructure();

    void removeSynapse(Synapse*synapse);
    void attachSynapse(Synapse*synapse);
    void activateComponent(Brain&, float signal);
	void fireNeuron(Synapse*target);
	void passDevelopmentSignal(float signal);
	ActivityStats& getActivityStats(){ return *this; }
	void developSynapse(const ActivityStats& stats);
	void visualizeTree(FILE *dendriteTree, size_t parent);
	Brain* getBrain() {return mBrain;}
};
}
#endif
