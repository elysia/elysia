#include "Platform.hpp"
#include "Branch.hpp"
#include "Synapse.hpp"
#include "ActivityStats.hpp"
#include "genome.pb.h"
#include "Brain.hpp"
namespace Elysia {

/**
 *	@param CellComponent *parent - parent branch of this branch
 *
 *	Description:	Constructor for Branch class.  Sets the parent CellComponent to
 *					some specified value
**/
Branch::Branch(CellComponent * parent) {
    mParentComponent=parent;
	mSignalWeight = 1.0f;
}

Branch::~Branch() {
	for(std::vector<Branch*>::iterator i=mChildBranches.begin(),ie=mChildBranches.end();
         i!=ie;
         ++i){
		delete *i;
	}
	for(std::vector<Synapse*>::iterator j=mChildSynapses.begin(),je=mChildSynapses.end();
         j!=je;
		 ++j){
		 delete *j;
	}
}


/**
 *	@returns protein density structure of the parent component of this branch
**/
ProteinDensity&Branch::getProteinDensityStructure() {
    return mParentComponent->getProteinDensityStructure();
}

/**
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
 *	Description:	Grows a new synapse on the branch
**/
void Branch::growSynapse(void){
	Synapse *s;
    mChildSynapses.push_back(s = new Synapse(this));
}

}
