#ifndef _ELYSIA_CELL_COMPONENT_HPP_
#define _ELYSIA_CELL_COMPONENT_HPP_
namespace Elysia {
class Neuron;
class ProteinDensity; 
class Branch;
class ActivityStats;
class Brain;
class BRAIN_CORE_EXPORT CellComponent{
protected:
    float mActivity;                   
    float mThreshold;                  //How much activity is required to fire
    float mSignalStrength;
    int   mDevelopmentStage;                        //0 = development, 1=mature
	SimTime   mLastActivity;				//timestep of last signal
protected:
    //Array of child branches
    std::vector<Branch *>mChildBranches;

public:
    CellComponent();
    virtual ~CellComponent(){}
    virtual Neuron*getParentNeuron()=0;
    void strengthen(float reward); //for punishment, use negative reward
	virtual ProteinDensity &getProteinDensityStructure()=0;
    virtual void activateComponent(Brain&,float signal)=0; //negative possible for inhibition
    virtual void syncBranchDensity(float parentRandomBranchDeterminer, float parentRandomDepthDeterminer, float baseBranchiness, float tipBranchiness, float treeDepth, int depth);
	virtual void passDevelopmentSignal(float signal)=0;
	virtual void developSynapse(const ActivityStats& stats)=0;
};

}
#endif
