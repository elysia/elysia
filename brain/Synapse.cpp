#include "Platform.hpp"
#include "Synapse.hpp"
#include "Neuron.hpp"
#include "genome.pb.h"
#include "ActivityStats.hpp"
#include "Brain.hpp"
//Takes care of disconnecting DendriteTip. Should be called in destructor

namespace Elysia {
	class Neuron;

Synapse::Synapse(CellComponent * parent){
	mParentBranch = parent;
	mBrain=parent->getParentNeuron()->getBrain();
    mWhere=mBrain->inactiveSynapse();
}

bool Synapse::detach(){
    if (mRecipientNeuron) {
        mRecipientNeuron->removeSynapse(this);
        mRecipientNeuron = NULL;
        return true;
    }
    return false;
}

void Synapse::connect(){
    Neuron *parentneuron;
    parentneuron = mParentBranch->getParentNeuron();
    //Get the target bound from gene
    //Pick location from bound
    //Nearest neighbor target
    mConnectionStrength = 1;
    mRecipientNeuron->attachSynapse(this);
}


bool pickrandomlocaton(Elysia::Genome::Gene gene, float age, Vector3f& retval){
    int numregions;
    numregions = gene.target_region_size();
    std::vector<BoundingBox3f3f> activeRegions;
    for(int i=0;i<numregions;i++){
        Elysia::Genome::TemporalBoundingBox boundingbox;
        boundingbox = gene.target_region(i);
        if((boundingbox.has_mint()==false||age>=boundingbox.mint()) && (boundingbox.has_maxt()==false||age<=boundingbox.maxt())) {
            activeRegions.push_back(BoundingBox3f3f(Vector3f(boundingbox.minx(),boundingbox.miny(),boundingbox.has_minz()?boundingbox.minz():0.0f),
                                                    Vector3f(boundingbox.maxx(),boundingbox.maxy(),boundingbox.has_maxz()?boundingbox.maxz():0.0f)));
        }
        //active regions
        
    }
    if (activeRegions.empty()) {
      return false;
    }
    unsigned int randomchoice = rand()%activeRegions.size();

    BoundingBox3f3f desiredregion = activeRegions[randomchoice];
    Vector3f delta =desiredregion.max() - desiredregion.min();
    Vector3f coord = Vector3f(delta.x*rand()/(float)RAND_MAX,
                              delta.y*rand()/(float)RAND_MAX,
                              delta.z*rand()/(float)RAND_MAX)+desiredregion.min();
    retval=coord;
    return true;
}

void Synapse::fireSynapse(float signal){
	signal += mSignalWeight;
	if(mFiringCounter == 0){
	  assert(mWhere==mBrain->inactiveSynapse());
	  mBrain->activeSynapse(this);
	}
	mFiringCounter = mFiringWindow;
    //Tick function determines when to fire the synapses from the neurons
    mParentBranch->activateComponent(*mBrain, signal);
    
}



void Synapse::develop(const ActivityStats& stats){
	float strengthenAmount=0.0f;
	int earlyDevelopmentWindow = 40;	//How many concurrent synapses firing is required to move to "mid-development"
	float initialStrengthen = 0.04f;		//How much to strengthen a firing synapse in early development
	float initialWeaken = -0.01f;
	float changeSize = 0.1f;				//Controls how quickly strengthening and weakening happen in mid-development
	float maxStrengthen = 0.1f;			//Maximum amount to strengthen in mid-development
	float maxWeaken = -0.04f;			//Maximum amount to weaken in mid-development
	float strengthenRange = 2.0f;		//The multiplier to the level of signal that determines how much and whether to strengthen the synapse 
	if(stats.mBestDevelopmentSignal < earlyDevelopmentWindow){				//Neuron still in early state
		if(mFiringCounter > 0){							//If the synapse is active and not helping the neuron, weaken. If it is active and is helping, strengthen
			mConnectionStrength += initialStrengthen;					//Strengthen weakly in beginning
		}
		else{
			mConnectionStrength += initialWeaken;
		}
	}
	else{
		if(stats.mBestDevelopmentSignal >= earlyDevelopmentWindow ){
			if(mFiringCounter > 0){
				strengthenAmount = changeSize*(strengthenRange*stats.mDevelopmentSignal - stats.mBestDevelopmentSignal)/(stats.mDevelopmentSignal+0.001f);
				if(strengthenAmount > maxStrengthen){strengthenAmount = maxStrengthen;}
				if(strengthenAmount < maxWeaken){strengthenAmount = maxWeaken;}
				mConnectionStrength += strengthenAmount;
			}
			else{
				strengthenAmount = stats.mDevelopmentSignal/(10*stats.mBestDevelopmentSignal);
				if(strengthenAmount < maxWeaken){strengthenAmount = maxWeaken;}
				mConnectionStrength += strengthenAmount;
			}
		}
		else{ assert(0);}
	}
}
	


}
    

    
