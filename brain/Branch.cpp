#include "Platform.hpp"
#include "Branch.hpp"
#include "Synapse.hpp"
#include "ActivityStats.hpp"
#include "genome.pb.h"
#include "Brain.hpp"
namespace Elysia {
Branch::Branch(CellComponent * parent) {
    mParentComponent=parent;
}

ProteinDensity&Branch::getProteinDensityStructure() {
    return mParentComponent->getProteinDensityStructure();
}

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

void Branch::passDevelopmentSignal(float signal){
    if(mActivity > mThreshold){
		signal += mSignalWeight;
    }
	mParentComponent->passDevelopmentSignal(signal);
}

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



void Branch::visualizeTree(FILE *dendriteTree, size_t parent){
    size_t self;
    self = size_t(this);
    fprintf(dendriteTree,"%lu -- %lu;\n",parent,self);
    
	for (std::vector<Branch*>::iterator i=mChildBranches.begin(),ie=mChildBranches.end();
         i!=ie;
         ++i)
	(*i)->visualizeTree(dendriteTree, self);

	/*
	for (std::vector<Synapse*>::iterator i=mChildSynapses.begin(),ie=mChildSynapses.end();
         i!=ie;
         ++i)
	(*i)->visualizeSynapse(dendriteTree, self);
	*/	
}

void Branch::growSynapse(void){
	Synapse *s;
    mChildSynapses.push_back(s = new Synapse(this));
}

}
