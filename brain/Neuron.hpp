#include <vector>

/**
 * The neuron class receives activity from the dendrite class and then passes activity onto the dendrite tips that connect to it
 */
class Neuron public CellComponent{
private:
    ///Neuron location
    Vector3f mNeuronLocation;
    Lobe *parent_lobe;
    //Array of child branches
    std::vector<Branch *>mChildBranches;
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
    float FiringTime
    float ReceptivityTime;
    float LearningResponsiveness
    float InhibitionSignal;
    float LearningResponsiveness;
    bool fire(Synapse *target);
    
public:
    Neuron(int number, int threshold, Lobe *lobe/*parent?*/, Vector3f location);
    void fire();
    ///Simulates one millisecond of neural time
    void tick();
    ProteinDensity& getProteindensityStructure();
    void removeSynapse(Synapse*synapse);
};
