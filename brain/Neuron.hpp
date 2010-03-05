#ifndef _ELYSIA_NEURON_HPP_
#define _ELYSIA_NEURON_HPP_
#include "CellComponent.hpp"
#include "ActivityStats.hpp"
#include <vector>
namespace Elysia {
class Synapse;
class ActivityStats;
/**
 * The neuron class receives activity from the dendrite class and then passes activity onto the dendrite tips that connect to it
 */
class Neuron : public CellComponent, ActivityStats{
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
	

    bool fireSynapse(Synapse *target);
    virtual Neuron*getParentNeuron(){return this;}

public:
    Neuron(float BaseBranchiness, float TipBranchiness, float TreeDepth, const Vector3f &location);
    void fire(Brain&);
    ///Simulates one millisecond of neural time
    void tick(Brain&);
    /* Removed to wait for tie in to protein soup
	ProteinDensity& getProteinDensityStructure();
	*/
    void removeSynapse(Synapse*synapse);
    void attachSynapse(Synapse*synapse);
    void activateComponent(Brain&,float signal);
	void fireNeuron(Brain&,Synapse*target);
	void passDevelopmentSignal(float signal);
	ActivityStats& getActivityStats(){ return *this; }
	void developSynapse(const ActivityStats& stats);
};
}
#endif
