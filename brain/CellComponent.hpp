#ifndef _ELYSIA_CELL_COMPONENT_HPP_
#define _ELYSIA_CELL_COMPONENT_HPP_

#define _M_ACTIVITY_DEFAULT_VALUE_			0
#define _M_THRESHOLD_DEFAULT_VALUE_			0.99
#define _M_DEVELOPMENT_STAGE_DEFAULT_VALUE_ 0

namespace Elysia {
class Neuron;
class ProteinDensity; 
class Branch;
class ActivityStats;
class Brain;
class BRAIN_CORE_EXPORT CellComponent{
protected:
    float mThreshold;                  //How much activity is required to fire
    float mActivity;                   
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
    virtual void syncBranchDensity(float parentRandomBranchDeterminer, float parentRandomDepthDeterminer, float baseBranchiness, float tipBranchiness, float treeDepth, float baseThreshold, float tipThreshold, int depth);
	void setActivity(float activity);
    float getActivity()const {return mActivity;}
    SimTime getLastActivity()const {
        return mLastActivity;
    }
    bool activityGreaterThanThreshold()const {return mActivity>mThreshold;}
    
    typedef std::vector<Branch*>::const_iterator ChildIterator;
    ChildIterator childBegin()const;
    ChildIterator childEnd()const;
};
}
#endif
