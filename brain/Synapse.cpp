#include "Platform.hpp"
#include "Synapse.hpp"
#include "Neuron.hpp"
#include "genome.pb.h"
#include "ActivityStats.hpp"
#include "Brain.hpp"
#include "SpatialSearch.hpp"

#define SIGNAL_WEIGHT	1.01f
//Takes care of disconnecting DendriteTip. Should be called in destructor

namespace Elysia {
	class Neuron;

/**
 *	Synapse::Synapse(CellComponent * parent)
 *
 *	@param CellComponent * parent - parent cell for synapse
 *
 *	Description:	Instantiates new Synapse object from parent component
**/
Synapse::Synapse(CellComponent * parent){
	mParentBranch = parent;
	mBrain=parent->getParentNeuron()->getBrain();
	mRecipientNeuron = NULL;
	mSignalWeight = SIGNAL_WEIGHT;	//This is the strength of the signal passed to activated branches. In the future, we may want to make it dynamic to allow certain synapses to have more force.
	mFiringWindow = 10;			//How long a neuron will fire for. FIXME: This needs to be passed from the gene someday 
	mFiringCounter = 0;
    mWhere=mBrain->activeSynapseListSentinel();
}

/**
 *	bool Synapse::detach()
 *
 *	@returns TRUE on success, otherwise false
 *
 *	Description:	Tries to deatch this synapse from some recipient neuron.
**/
bool Synapse::detach(){
    if (mRecipientNeuron) {
        mRecipientNeuron->removeSynapse(this);
        mRecipientNeuron = NULL;
        return true;
    }
    return false;
}

/**
 *	Synapse::connect()
 *	
 *	Description:	Detaches the synapse from a recipient neuron if it's connected to one already.
 *					Then finds the nearest neuron to it and attaches the synapse to it with connection
 *					strength 1.0
**/
void Synapse::connect(){
    if(mRecipientNeuron){this->detach();}
    Neuron * parentNeuron = mParentBranch->getParentNeuron();
    Brain *parentBrain = parentNeuron->getBrain();
    float age = parentBrain->getAge();
    Vector3f targetLocation = parentNeuron->getProteinDensityStructure().getRandomTargetPoint(age);
    mRecipientNeuron = parentBrain->getSpatialSearch()->findNearestNeighbor(targetLocation, parentNeuron);
    if (mRecipientNeuron) {
      //Nearest neighbor target
      mConnectionStrength = 1.0f;
      mRecipientNeuron->attachSynapse(this);
    }
}

/**
 *	bool pickrandomlocaton(Elysia::Genome::Gene gene, float age, Vector3f& retval)
 *
 *	@param Elysia::Genome::Gene gene - provides a target region size
 *	@param float age - age of target
 *	@param Vectore3f& retval - returned coordinates that should be randomized
 *	@returns TRUE on success, FALSE on failure
 *	
 *	Description:	
**/
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

/**
 *	Synapse::fireSynapse()
 *
 *	Description:	If the firing counter is not set, fire the synapse.
**/
void Synapse::fireSynapse(){

	if(mFiringCounter == 0){
	  //assert(mWhere= mBrain->activeSynapseListSentinel()); //Wait, why is this here?
	mWhere = mBrain->activateSynapse(this);
	}
	mFiringCounter = mFiringWindow;
    //Tick function determines when to fire the synapses from the neurons
}

/**
 *	Synapse::residualFire()
 *
 *	Description:	If the firing counter is positive, decrement it; deactivate the synapse if the firing counter is 
 *					then zero, then activate the brain component from the parent branch with mSignalWeight.
 *					If the firing counter is zero... what the hell does assert(this) do anyway?  
**/
void Synapse::residualFire(){
	if(mFiringCounter > 0){
		mFiringCounter--;
		if(mFiringCounter == 0){
			mBrain->inactivateSynapse(this);
		}
		mParentBranch->activateComponent(*mBrain, mSignalWeight);
	}
	else{
		assert(this);		// what is this doing here?
	}
}

/**
 *	void Synapse::develop(const ActivityStats& stats)
 *
 *	@param const ActivityStats& stats - activity statistics
 *
 *	Description:	If the neuron is in its early states then give it some initial strengthening or weakening.
 *					Otherwise, the connection is strengthened or weakened based on some other function (see code).
 *					If this synapse isn't connected to anything, then the above does not occur and the synapse is connected
 *					to a neuron based on the connect() function
**/
void Synapse::develop(const ActivityStats& stats){

	float strengthenAmount=_STRENGTHEN_AMOUNT_;
	int earlyDevelopmentWindow = _EARLY_DEV_WINDOW_;		//How many concurrent synapses firing is required to move to "mid-development"
	float initialStrengthen = _INITIAL_STRENGTHEN_;			//How much to strengthen a firing synapse in early development
	float initialWeaken = _INITIAL_WEAKEN_;
	float changeSize = _CHANGE_SIZE_;				//Controls how quickly strengthening and weakening happen in mid-development
	float maxStrengthen = _MAX_STRENGTHEN_;			//Maximum amount to strengthen in mid-development
	float maxWeaken = _MAX_WEAKEN_;					//Maximum amount to weaken in mid-development
	float strengthenRange = _STRENGTHEN_RANGE_;		//The multiplier to the level of signal that determines how much and whether to strengthen the synapse 

    if(mRecipientNeuron){
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
	    	else{ assert(0);}		// there are probably better ways to do this
	    }
    }
    else{
        this->connect();
    }
}

/**
 *	Synapse::visualizeSynapse(FILE *dendriteTree, size_t parent)
 *
 *	@param FILE *dendriteTree - output file
 *	@param size_t parent - size of parent 
 *
 *	Description:	Prints this synapse to a file
**/
void Synapse::visualizeSynapse(FILE *dendriteTree, size_t parent){
    size_t self;
	size_t recipient;
    self = size_t(this);
	recipient = size_t(mRecipientNeuron);
	fprintf(dendriteTree,"%lu [style=\"filled\" fillcolor=\"#00FF00\"];\n",self);
	fprintf(dendriteTree,"%lu -- %lu;\n",parent,self);
	fprintf(dendriteTree,"%lu [style=\"filled\" fillcolor=\"#FF0000\"];\n",recipient);
	fprintf(dendriteTree,"%lu -- %lu;\n",self,recipient);
}

}
    

    
