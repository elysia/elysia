#include "Platform.hpp"
#include "Synapse.hpp"
#include "Neuron.hpp"
#include "genome.pb.h"
#include "ActivityStats.hpp"
#include "Brain.hpp"
#include "SpatialSearch.hpp"
#include "Development.hpp"
#define SIGNAL_WEIGHT	1.01f
//Takes care of disconnecting DendriteTip. Should be called in destructor

namespace Elysia {
	class Neuron;

/**
 *	@param CellComponent * parent - parent cell for synapse
 *
 *	Description:	Instantiates new Synapse object from parent component
**/
Synapse::Synapse(CellComponent * parent){
	mParentBranch = parent;
	mBrain=parent->getParentNeuron()->getBrain();
	mRecipientNeuron = NULL;
	mParentNeuron = mParentBranch->getParentNeuron();
	mSignalWeight = SIGNAL_WEIGHT;	//This is the strength of the signal passed to activated branches. In the future, we may want to make it dynamic to allow certain synapses to have more force.
	mFiringWindow = 10;			//How long a neuron will fire for. FIXME: This needs to be passed from the gene someday 
	mFiringCounter = 0;
    mDevelopmentStage = 0;
	mWhere=mBrain->activeSynapseListSentinel();
}


void Synapse::setDevelopmentStage(int stage){
	mDevelopmentStage = stage;
}

int Synapse::getDevelopmentStage(){
	return mDevelopmentStage;
}

Synapse::~Synapse(){
	bool a=detach();
	mBrain->inactivateSynapse(this);
	//mBrain->deleteSynapse(this);		//Removed when AllSynapse list was removed
	if(!a){
		assert(this);
	}
}

/**
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
 *	Description:	Detaches the synapse from a recipient neuron if it's connected to one already.
 *					Then finds the nearest neuron to it and attaches the synapse to it with connection
 *					strength 1.0
**/
void Synapse::connect(){
    if(mRecipientNeuron){this->detach();}
	//I have removed this line because I decided to give synapses their parent neuron in their class
    //Neuron * parentNeuron = mParentBranch->getParentNeuron();
    Brain *parentBrain = mParentNeuron->getBrain();
    float age = parentBrain->getAge();
    Vector3f targetLocation = mParentNeuron->getProteinDensityStructure().getRandomTargetPoint(age);
    mRecipientNeuron = static_cast<Neuron*>(parentBrain->getSpatialSearch()->findNearestNeighbor(targetLocation, mParentNeuron));
    if (mRecipientNeuron) {
      //Nearest neighbor target
      mConnectionStrength = 1.0f;
      mRecipientNeuron->attachSynapse(this);
    }
}

/**
 *	@param Elysia::Genome::Gene gene - provides a target region size
 *	@param float age - age of target
 *	@param Vectore3f& retval - returned coordinates that should be randomized
 *	@returns TRUE on success, FALSE on failure
 *	
 *	Description:	Umm... I don't know.  I think it makes cheeseburgers
**/
bool pickrandomlocaton(Elysia::Genome::Gene gene, float age, Vector3f& retval){
    int numregions;
    numregions = gene.dendrite_region_size();
    std::vector<BoundingBox3f3f> activeRegions;
    for(int i=0;i<numregions;i++){
        Elysia::Genome::TemporalBoundingBox boundingbox;
        boundingbox = gene.dendrite_region(i);
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
 *	Description:	If the firing counter is not set, fire the synapse.
**/
void Synapse::fireSynapse(){

	if(mFiringCounter == 0){
		float receivedFrequency = mRecipientNeuron->getCurrentOutputFrequency();
		Neuron::MinMax acceptedFrequencies = mParentNeuron->getCurrentInputBound();
		if(receivedFrequency >= acceptedFrequencies.min && receivedFrequency <= acceptedFrequencies.max){
			mWhere = mBrain->activateSynapse(this);
		}
	}
	mFiringCounter = mFiringWindow;
    //Tick function determines when to fire the synapses from the neurons
}

/**
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
        mParentBranch->getParentNeuron()->development()->passDevelopmentSignal(this,mParentBranch,mSignalWeight);
	}
	else{
		assert(this);		//A synapse should not be on the process synapse list if its counter is 0
	}
}


/**
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
    

    
