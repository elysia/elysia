#include "Platform.hpp"
#include "Branch.hpp"
#include "Synapse.hpp"
#include "ActivityStats.hpp"
namespace Elysia {
Branch::Branch(CellComponent * parent) {
    mParentComponent=parent;
}

ProteinDensity&Branch::getProteinDensityStructure() {
    return mParentComponent->getProteinDensityStructure();
}

void Branch::activateComponent(float signal){
	if(mLastActivity != GLOBAL_TIME){
		mLastActivity = GLOBAL_TIME;
		mActivity = 0;
	}
	mActivity += signal;
    if(mActivity > mThreshold){
        mParentComponent->activateComponent(mSignalWeight);
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

void Branch::growSynapse(void){
	Synapse *s;
    mChildSynapses.push_back(s = new Synapse(this));
}

}
