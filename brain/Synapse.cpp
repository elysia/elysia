#include "Platform.hpp"
#include "Synapse.hpp"
#include "Neuron.hpp"
#include "genome.pb.h"
//Takes care of disconnecting DendriteTip. Should be called in destructor

namespace Elysia {
	class Neuron;

Synapse::Synapse(CellComponent * parent){
		mParentBranch = parent;
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
            activeRegions.push_back(BoundingBox3f3f(Vector3f(boundingbox.minx(),boundingbox.miny(),boundingbox.has_minz()?boundingbox.minz():0.0),
                                                    Vector3f(boundingbox.maxx(),boundingbox.maxy(),boundingbox.has_maxz()?boundingbox.maxz():0.0)));
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
	mFiringCounter = mFiringWindow;
    //Tick function determines when to fire the synapses from the neurons
    mParentBranch->activateComponent(signal);
}

void Synapse::develop(ActivityStats stats){
	Neuron *parent;  
	float strengthenamount=0;
	if(stats.mBestDevelopmentSignal < 20){		//Neuron still in early state
		if(mFiringCounter > 0){							//If the synapse is active and not helping the neuron, weaken. If it is active and is helping, strengthen
			mConnectionStrength +=0.04;					//Strengthen weakly in beginning
		}
		else{
			mConnectionStrength -= 0.01;
		}
	}
	else{
		if(stats.mBestDevelopmentSignal >= 20 ){
			if(mFiringCounter > 0){
				strengthenamount = (2*stats.mDevelopmentSignal - stats.mBestDevelopmentSignal)/(10*stats.mDevelopmentSignal+0.001);
				if(strengthenamount > 0.1){strengthenamount = 0.1;}
				if(strengthenamount < -0.02){strengthenamount = -0.02;}
				mConnectionStrength += strengthenamount;
			}
			else{
				strengthenamount = stats.mDevelopmentSignal/(10*stats.mBestDevelopmentSignal);
				if(strengthenamount < -0.02){strengthenamount = -0.02;}
				mConnectionStrength += strengthenamount;
			}
		}
	}
}
	


}
    

    
