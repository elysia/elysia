#include "Platform.hpp"
#include "Neuron.hpp"
#include "Synapse.hpp"
#include "Branch.hpp"
#include "ActivityStats.hpp"
#include "genome.pb.h"
#include "Brain.hpp"
#include "SimpleProteinEnvironment.hpp"
#include "ProteinDensity.hpp"
#include "SpatialSearch.hpp"
#include <time.h>

namespace Elysia {

/**
 *	Description:	Deletes the protein density from memory and removes this neuron from the nearest-neighbor search
**/
Neuron::~Neuron() {
    for (std::ptrdiff_t i=(std::ptrdiff_t)mConnectedSynapses.size()-1;i>=0;--i) {
        mConnectedSynapses[i]->detach();
    }
    assert(mConnectedSynapses.size()==0);
    delete mProteinDensity;
    mBrain->getSpatialSearch()->removeNeighbor(this);
	for(std::vector<Branch*>::iterator i=mChildBranches.begin(),ie=mChildBranches.end();
         i!=ie;
         ++i){
		delete *i;
	}
}

/**
 *	@param Brain* brain - inherits a parent brain structure from somewhere
 *	@param float BaseBranchiness - 
 *	@param float TipBranchiness - 
 *	@param float TreeDepth - 
 *	@param const Vector3f &location - neuron's location in space
 *	@param const Elysia::Genome::Gene &gene - parent gene info
 *
 *	Description:	Instantiates a new neuron object at some phyiscal location, setting the development counter at zero.
 *					The branch density is synched based upon some random parameters and some base parameters, as well as a defined tree depth
 *					This neuron is added to the nearest neighbor spatial search
**/
Neuron::Neuron(Brain* brain, float BaseBranchiness, float TipBranchiness, float TreeDepth, const Vector3f &location, const Elysia::Genome::Gene&gene):  mNeuronLocation(location){
    mProteinDensity = new ProteinDensity(brain->getProteinEnvironment(),gene);
    mBrain=brain;
    mDevelopmentCounter = 0;
    mWhere=brain->activeNeuronListSentinel();
    mRandomDepthDeterminer=rand()/(float)RAND_MAX;
    mRandomBranchDeterminer=rand()/(float)RAND_MAX;
    //mWhere=mBrain->activateNeuron(this); //JUST FOR TESTING REMOVE ME LATER
    this->syncBranchDensity(mRandomBranchDeterminer, mRandomDepthDeterminer, BaseBranchiness, TipBranchiness, TreeDepth, 0);
    mBrain->getSpatialSearch()->addNeighbor(this);
	mBaseBranchiness = BaseBranchiness;
    mTipBranchiness = TipBranchiness;
    mTreeDepth = TreeDepth;
	/* These are not yet used
    mBaseThreshold;
    mTipThreshold;
	*/
}

/**
 *	Description:	Fires neuron into all synapses connected to it
**/
void Neuron::fire() {
	printf("fire");
    for (std::vector<Synapse*>::iterator i=mConnectedSynapses.begin(),ie=mConnectedSynapses.end();
         i!=ie;
         ++i) {
        this->fireNeuron(*i);
    }
}

/**
 *	@param Brain& - unused, here for inheritance
 *	@param float signal - signal to add to activity
 *
 *	Description:	If last activity is not current sim time in the brain, reset activity to zero
 *					and make last activity time current brain time. 
 *					Then, if the activity is under some treshold, the signal parameter is added to activity,
 *					after which the neuron is activated if the new activity level is above some threshold
**/
void Neuron::activateComponent(Brain&, float signal){
	if(mLastActivity != mBrain->mCurTime){
		mLastActivity = mBrain->mCurTime;
		mActivity = 0;
	}
	if(mActivity <= mThreshold){
	    mActivity += signal;
		if(mActivity > mThreshold){
			mBrain -> activateNeuron(this);     //Add to Neuron List
		}
	}
}

/**
 *	@param Synapse *synapse - pointer to some synapse you want to remove
 *
 *	Description:	Looks for the provided synapse in the connected synapses vector and removes it, if found.
 *					Prints an error message if not found
**/
void Neuron::removeSynapse(Synapse*synapse){
  std::vector<Synapse* >::iterator where=std::find(mConnectedSynapses.begin(),mConnectedSynapses.end(),synapse);
  if (where!=mConnectedSynapses.end()) {
    mConnectedSynapses.erase(where);
  }else {
    std::cerr<< "Could not find synapse\n";
  }
}

/**
 *	@param Synapse *target - pointer to a synapse to fire
 *
 *	Description:	Fires some synapse
**/
void Neuron::fireNeuron(Synapse*target){
	target->fireSynapse();			
}

/**
 *	@param Synapse *target - pointer to a synapse to add
 *
 *	Description:	Puts the new synapse at the end of the connected synapses vector
**/
void Neuron::attachSynapse(Synapse*target){
	mConnectedSynapses.push_back(target);
}

/**
 *	@param float signal - signal to add to development signal
 *
 *	Description:	Adds some value to the development signal level
**/
void Neuron::passDevelopmentSignal(float signal){
	mDevelopmentSignal += signal;
}
/**
 *	@returns *mProteinDensity
**/
ProteinDensity& Neuron::getProteinDensityStructure(){
    return *mProteinDensity;
}

/**
 *	@param const ActivityStats& stats - activity statistics for something
 *
 *	Description:	Develops all synapses attached as child branches of this neuron
**/
void Neuron::developSynapse(const ActivityStats& stats){
for (std::vector<Branch*>::iterator i=mChildBranches.begin(),ie=mChildBranches.end();
         i!=ie;
         ++i)
	(*i)->developSynapse(stats);
}

/**
 *	@param FILE *dendriteTree - output file for the visualized dendrite tree
 *	@param size_t parent - unused, please remove
 *
 *	Description:	Outputs a visualization of the dendrite tree to a file
**/
void Neuron::visualizeTree(FILE *dendriteTree, size_t parent){
    
    size_t self;
	
    self = (size_t)this;
    //fprintf(dendriteTree,"Graph Tree {\n");
    for (std::vector<Branch*>::iterator i=mChildBranches.begin(),ie=mChildBranches.end();
         i!=ie;
         ++i)
         (*i) -> visualizeTree(dendriteTree, self);
   //fprintf(dendriteTree,"}");
   
   }

/**
 *	Description:	Executes one "tick" of the brain simulation for this neuron.
 *					If activity is over some threshold, fires the neuron.
 *					If development stage is not set and development counter is not at zero,
 *					the development counter is decremented.  If it is at zero, then we developSynapse(getActivityStats())
 *					and reset development counter to 30.
 *					mActivity always cleared at the end of this
**/
void Neuron::tick(){
	if(mActivity > mThreshold){
		fire();
	}
	if(mDevelopmentStage == 0){
		if(mDevelopmentCounter == 0){
			ActivityStats& stats = getActivityStats();
			developSynapse(stats);
			mDevelopmentCounter	= 30;					//number of timesteps before next development re-evaluation
		}
		else{ mDevelopmentCounter--;}
	}
	mActivity = 0;
}
}
