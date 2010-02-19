#ifndef _ELYSIA_NEURON_HPP_
#define _ELYSIA_NEURON_HPP_
#include "CellComponent.hpp"
#include <vector>
namespace Elysia {
class Synapse;
/**
 * The neuron class receives activity from the dendrite class and then passes activity onto the dendrite tips that connect to it
 */
class Neuron : public CellComponent{
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
    float FiringTime;
    float ReceptivityTime;
    float LearningResponsiveness;
    float InhibitionSignal;
    float mRandomDepthDeterminer;
    float mRandomBranchDeterminer;
	float mNeuronSignalWeight;

    bool fireSynapse(Synapse *target);
    virtual Neuron*getParentNeuron(){return this;}

public:
    Neuron(float BaseBranchiness, float TipBranchiness, float TreeDepth, const Vector3f &location);
    void fire();
    ///Simulates one millisecond of neural time
    void tick();
    /* Removed to wait for tie in to protein soup
	ProteinDensity& getProteinDensityStructure();
	*/
    void removeSynapse(Synapse*synapse);
    void attachSynapse(Synapse*synapse);
    void activateComponent(float signal);
	void fireNeuron(Synapse*target);
};
}
#endif
