#include <vector>

//The branch class covers all branch points in the dendrite tree. When a dendrite is active, it passes activity
//to the previous dendrite branch (whose identity it stores as a pointer). Neurons at the base 
class Branch public CellComponent{
    int mSignalStrength;
    
    CellComponent *mParentComponent;
    //Which branch does this synapse belong to
    std::vector<Synapse *>mChildSynapses;
    //Which dendrite tips come from this branch
	std::vector<Branch *>mChildBranches

   public:
    Branch(CellComponent*parent);
    virtual ProteinDensity&getProteinDensityStructure();
    //Pointers to branches that come off of this branch
    virtual void growBranch(float EarlyBranchiness, float LateBranchiness, int TreeDepth);
    void growSynapse();
};

