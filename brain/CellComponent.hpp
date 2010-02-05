#ifndef _ELYSIA_CELL_COMPONENT_HPP_
#define _ELYSIA_CELL_COMPONENT_HPP_
namespace Elysia {
class Neuron;
class ProteinDensity; 
class Branch;

class CellComponent{
protected:
    float mActivity;                   
    float mThreshold;                  //How much activity is required to fire
    float mSignalStrength;
    int   mDevelopmentStage;                        //0 = development, 1=mature

protected:
    //Array of child branches
    std::vector<Branch *>mChildBranches;

public:
    CellComponent();
    virtual Neuron*getParentNeuron()=0;
    void strengthen(float reward); //for punishment, use negative reward
    virtual ProteinDensity &getProteinDensityStructure()=0;
    virtual void activateComponent(float signal)=0; //negative possible for inhibition
    virtual void syncBranchDensity(float parentRandomBranchDeterminer, float parentRandomDepthDeterminer, float baseBranchiness, float tipBranchiness, float treeDepth, int depth);
};

}
#endif
