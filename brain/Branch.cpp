#include "Platform.hpp"
#include "Branch.hpp"
#include "Synapse.hpp"
#include "ActivityStats.hpp"
#include "genome.pb.h"
#include "Brain.hpp"
namespace Elysia {

/**
 *	Branch::Branch
 *
 *	@param CellComponent *parent - parent branch of this branch
 *
 *	Description:	Constructor for Branch class.  Sets the parent CellComponent to
 *					some specified value
**/
Branch::Branch(CellComponent * parent) {
    mParentComponent=parent;
}

/**
 *	ProteinDensity&Branch::getProteinDensityStructure()
 *	
 *	@return protein density structure of the parent component of this branch
**/
ProteinDensity&Branch::getProteinDensityStructure() {
    return mParentComponent->getProteinDensityStructure();
}

/**
 *	Branch::activateComponent
 *
 *	@param Brain &brain - address of a brain in memory
 *	@param float signal - numerical signal level to add to brain activity
 *
 *	Description:	If the time of the last activity is not the current time of the brain
 *					then sets last activity time to brain's current time and resets activity
 *					signal level to zero.
 *					Then adds signal parameter to activity level.
 *					If the activity is above some threshold, then the branch's parent component
 *					activates a brain and passes a predefined signal to it.  The branch is then 
 *					prevented from firing again this round.
**/
void Branch::activateComponent(Brain&brain, float signal){
	if(mLastActivity != brain.mCurTime){
		mLastActivity = brain.mCurTime;
		mActivity = 0;
	}
	mActivity += signal;
    if(mActivity > mThreshold){
        mParentComponent->activateComponent(brain,mSignalWeight);
		mActivity = -999.9f;					//prevent branch from firing again this round
    }
}

/**
 *	Branch::passDevelopmentSignal
 *
 *	@param float signal - signal to pass to parent component
 *
 *	Description:	If the activity is above a threshold, then add weight to the signal.
 *					Passes this signal argument, with possible added weight, to its parent
 *					component.
**/
void Branch::passDevelopmentSignal(float signal){
    if(mActivity > mThreshold){
		signal += mSignalWeight;
    }
	mParentComponent->passDevelopmentSignal(signal);
}

/**
 *	Branch::developSynapse
 *
 *	@param const ActivityStats& stats - activity statistics for this synapse
 *
 *	Description:	For all the child synapses of this branch, call their develop() methods. 
 *					Then call this function for all child synapses, using the argument passed
 *					to this function as the argument for the above two.
**/
void Branch::developSynapse(const ActivityStats& stats){
	
	for (std::vector<Synapse*>::iterator i=mChildSynapses.begin(),ie=mChildSynapses.end();
         i!=ie;
         ++i)
		(*i)->develop(stats);
	
	for (std::vector<Branch*>::iterator i=mChildBranches.begin(),ie=mChildBranches.end();
         i!=ie;
         ++i)
		(*i)->developSynapse(stats);
}


/**
 *	Branch::visualizeTree
 *
 *	@param FILE *dendriteTree - output file for visualization
 *	@param size_t parent  - size of the parent branch
 *
 *	Description:	Outputs a visualization of the branch tree to a file
**/
void Branch::visualizeTree(FILE *dendriteTree, size_t parent){
    size_t self;
    self = size_t(this);
	fprintf(dendriteTree,"%lu [style=\"filled\" fillcolor=\"#0000FF\"];\n",self);
    fprintf(dendriteTree,"%lu -- %lu;\n",parent,self);
    
	for (std::vector<Branch*>::iterator i=mChildBranches.begin(),ie=mChildBranches.end();
         i!=ie;
         ++i)
	(*i)->visualizeTree(dendriteTree, self);

	
	for (std::vector<Synapse*>::iterator i=mChildSynapses.begin(),ie=mChildSynapses.end();
         i!=ie;
         ++i)
	(*i)->visualizeSynapse(dendriteTree, self);
		
}

/**
 *	Branch::growSynapse
 *
 *	Description:	Grows a new synapse on the branch
**/
void Branch::growSynapse(void){
	Synapse *s;
    mChildSynapses.push_back(s = new Synapse(this));
}

}
