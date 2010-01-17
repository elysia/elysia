#include <vector>

//The branch class covers all branch points in the dendrite tree. When a dendrite is active, it passes activity
//to the previous dendrite branch (whose identity it stores as a pointer). Neurons at the base 
class Branch public CellComponent{
    int mSignalStrength;
    std::vector<Branch *>mUpstreamBranches;   //0 for a branch at the base, signifying to fire the neuron
    CellComponent *mParentComponent;
    //Which neuron does this synapse belong to
    std::vector<Synapse *>mChildSynapse;
        //Which dendrite tips come from this branch
	std::vector<Branch *>mChildBranch
    void strengthen(Branch *child_branch, Synapse *childSynapseTip, float reward); //for punishment, use negative reward
public:
    Branch(CellComponent*parent);
    ProteinDensity&getProteinDensityStructure();
    //Pointers to branches that come off of this branch
    void activate(float excitation);
    void fire();
};
