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
#include "Development.hpp"

//Fixme: instaed include development factory by which we pull out NaiveDevelopment
#include "DevelopmentFactory.hpp"
#include <time.h>

namespace Elysia {

/**
 *	Description:	Deletes the protein density from memory and removes this neuron from the nearest-neighbor search
**/
Neuron::~Neuron() {
    ProteinEnvironment::iterator iter = mBrain->getProteinEnvironment()->getIterator(mNeuronLocation);
    //assert(iter!=mBrain->getProteinEnvironment()->end()); commented uot for test since we are not making genes in the standard way
    if (iter!=mBrain->getProteinEnvironment()->end()) {
        mBrain->decrementNumNeurons(iter);//decrement density count
    }
    for (std::ptrdiff_t i=(std::ptrdiff_t)mConnectedSynapses.size()-1;i>=0;--i) {
        mConnectedSynapses[i]->detach();
    }
	mBrain->inactivateNeuron(this);
	mBrain->deleteNeuron(this);
    assert(mConnectedSynapses.size()==0);
    mBrain->getSpatialSearch()->removeNeighbor(this);
    mBrain->notifyPluginsNeuronDestruction(this);
    delete mProteinDensity;
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
Neuron::Neuron(Brain* brain, float BaseBranchiness, float TipBranchiness, float TreeDepth, float BaseThreshold, float TipThreshold, const Vector3f &location, const Elysia::Genome::Gene&gene):  mNeuronLocation(location){
    
    mDevelopment = DevelopmentFactory::getSingleton().getConstructor("naive")();//FIXME have a mechanism for using the gene to select the string "naive"
    mDevelopment->setParent(this);
    mProteinDensity = new ProteinDensity(brain->getProteinEnvironment(),gene);
    mBrain=brain;
    mWhere=brain->activeNeuronListSentinel();
    mRandomDepthDeterminer=rand()/(float)RAND_MAX;
    mRandomBranchDeterminer=rand()/(float)RAND_MAX;
    this->syncBranchDensity(mRandomBranchDeterminer, mRandomDepthDeterminer, BaseBranchiness, TipBranchiness, TreeDepth, BaseThreshold, TipThreshold, 0);
    mBrain->getSpatialSearch()->addNeighbor(this);
	mBaseBranchiness = BaseBranchiness;
    mTipBranchiness = TipBranchiness;
    mTreeDepth = TreeDepth;
    ProteinEnvironment::iterator iter=brain->getProteinEnvironment()->getIterator(mNeuronLocation);
    //assert(iter!=brain->getProteinEnvironment()->end());		//Commented out for test, since we are not making genes in the standard way during testing
    if (iter!=brain->getProteinEnvironment()->end()) {
        mBrain->incrementNumNeurons(iter);//increment density count
    }
    mBaseThreshold = BaseThreshold;
    mTipThreshold = TipThreshold;

	if(BaseBranchiness + TipBranchiness == 0){
		type = 0;
	}
	else{type = 1;}
	
}

/**
 *	Description:	Fires neuron into all synapses connected to it
**/
void Neuron::fire() {
	if(type == 1){
	printf("fire");
	}
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
 *	@returns *mProteinDensity
**/
ProteinDensity& Neuron::getProteinDensityStructure(){
    return *mProteinDensity;
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
    mDevelopment->develop();
	mActivity = 0;
}





}
