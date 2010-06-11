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
Neuron::~Neuron() {
    delete mProteinDensity;
    mBrain->getSpatialSearch()->removeNeighbor(this);
}
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
}
void Neuron::fire() {
    for (std::vector<Synapse*>::iterator i=mConnectedSynapses.begin(),ie=mConnectedSynapses.end();
         i!=ie;
         ++i) {
        this->fireNeuron(*i);
    }
}

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

void Neuron::removeSynapse(Synapse*synapse){
  std::vector<Synapse* >::iterator where=std::find(mConnectedSynapses.begin(),mConnectedSynapses.end(),synapse);
  if (where!=mConnectedSynapses.end()) {
    mConnectedSynapses.erase(where);
  }else {
    std::cerr<< "Could not find synapse\n";
  }
}

void Neuron::fireNeuron(Synapse*target){
	target->fireSynapse();			
}

void Neuron::attachSynapse(Synapse*target){
	mConnectedSynapses.push_back(target);
}

void Neuron::passDevelopmentSignal(float signal){
	mDevelopmentSignal += signal;
}
ProteinDensity& Neuron::getProteinDensityStructure(){
    return *mProteinDensity;
}
void Neuron::developSynapse(const ActivityStats& stats){
for (std::vector<Branch*>::iterator i=mChildBranches.begin(),ie=mChildBranches.end();
         i!=ie;
         ++i)
	(*i)->developSynapse(stats);
}

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
