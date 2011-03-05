#ifndef _ELYSIA_NEURON_HPP_
#define _ELYSIA_NEURON_HPP_
#include "Placeable.hpp"
#include "CellComponent.hpp"
#include "ActivityStats.hpp"

#include <vector>

namespace Elysia {
namespace Genome {
class Gene;
}

class Synapse;
class ActivityStats;
class Development;
/**
 * The neuron class receives activity from the dendrite class and then passes activity onto the dendrite tips that connect to it
 */
class BRAIN_CORE_EXPORT Neuron : public Placeable, public CellComponent, ActivityStats{
    ProteinDensity *mProteinDensity;
    std::list<Neuron*>::iterator mWhere;
    friend class Brain;
    /////WE NO LONGER USE LOBES AT LEAST FOR NOWLobe *parent_lobe;
    //Array of dendrites connections
    std::vector<Synapse* >mConnectedSynapses;
    /**
     * Fire activates a DendriteTip
     * \param target is the DendriteTip that should be activated
     * \returns whether the signal successfully passed through the inter-neuron gap
     */
    float mBaseBranchiness;
    float mTipBranchiness;
    float mTreeDepth;
    float mBaseThreshold;
    float mTipThreshold;
    float mFiringTime;			//Holding signal could be done solely on the synapse. Presently unused
    float mReceptivityTime;
    float mLearningResponsiveness;
    float mInhibitionSignal;
    float mRandomDepthDeterminer;
    float mRandomBranchDeterminer;
	float mNeuronSignalWeight;
	int type;							//Input neuron = 0, otherwise = 1
    Development*mDevelopment;
    virtual Neuron*getParentNeuron(){return this;}
    
public:
    Neuron(Brain * brain, float BaseBranchiness, float TipBranchiness, float TreeDepth, float BaseThreshold, float TipThreshold, const Vector3f &location, const Elysia::Genome::Gene&spawningGene);
    ~Neuron();
    void fire();
    ///Simulates one millisecond of neural time
    void tick();

	ProteinDensity& getProteinDensityStructure();
    std::string getName() const {return std::string();}
    void removeSynapse(Synapse*synapse);
    void attachSynapse(Synapse*synapse);
    void activateComponent(Brain&, float signal);
	void fireNeuron(Synapse*target);
	ActivityStats& getActivityStats(){ return *this; }
	void visualizeTree(FILE *dendriteTree, size_t parent);
    Development*development(){
        return mDevelopment;
    }
    std::vector<Synapse*>::iterator getSynapsesAtAxonTipBegin(){
        return mConnectedSynapses.begin();
    }
    std::vector<Synapse*>::iterator getSynapsesAtAxonTipEnd(){
        return mConnectedSynapses.end();
    }
};
}
#endif
