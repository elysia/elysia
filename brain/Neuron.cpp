                                                                                                                                                                                                                                                                                                                       #include "Platform.hpp"
#include "Neuron.hpp"
#include "Synapse.hpp"
#include "Branch.hpp"
#include "ActivityStats.hpp"

namespace Elysia {

Neuron::Neuron(float BaseBranchiness, float TipBranchiness, float TreeDepth, const Vector3f &location):  mNeuronLocation(location){

    mRandomDepthDeterminer=rand()/(float)RAND_MAX;
    mRandomBranchDeterminer=rand()/(float)RAND_MAX;
    this->syncBranchDensity(mRandomBranchDeterminer, mRandomDepthDeterminer, BaseBranchiness, TipBranchiness, TreeDepth, 0);        
}
void Neuron::fire() {
    for (std::vector<Synapse*>::iterator i=mConnectedSynapses.begin(),ie=mConnectedSynapses.end();
         i!=ie;
         ++i) {
        this->fireNeuron(*i);
    }
}

void Neuron::activateComponent(float signal){
    mActivity += signal;
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
	target->fireSynapse(mNeuronSignalWeight);			
}

void Neuron::attachSynapse(Synapse*target){
	mConnectedSynapses.push_back(target);
}

void Neuron::passDevelopmentSignal(float signal){
	mDevelopmentSignal += signal;
}

void Neuron::developSynapse(const ActivityStats& stats){
for (std::vector<Branch*>::iterator i=mChildBranches.begin(),ie=mChildBranches.end();
         i!=ie;
         ++i)
	(*i)->developSynapse(stats);
}
void Neuron::tick(){

	if(mDevelopmentStage == 0){
		if(mDevelopmentCounter == 0){
			ActivityStats& stats = getActivityStats();
			developSynapse(stats);
			mDevelopmentCounter	= 30;					//number of timesteps before next development re-evaluation

		}
		else{ mDevelopmentCounter--;}
	}
}
}
