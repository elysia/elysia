#include "NaiveDevelopment.hpp"
#include "DevelopmentFactory.hpp"
namespace Elysia {
static Development*makeNaiveDevelopment() {
    return new NaiveDevelopment;
}

static bool initiateNaiveDevelopment() {
    DevelopmentFactory::getSingleton().registerConstructor ("naive",&makeNaiveDevelopment,true);
    return true;
}

NaiveDevelopment::NaiveDevelopment() {
    mDevelopmentSignal=0;
}

/**
 *	@param float signal - signal to pass to parent component
 *
 *	Description:	For Neurons: end recursion and store the development signal
 *                  For Branches: If the activity is above a threshold, then add weight to the signal.
 *					Passes this signal argument, with possible added weight, to its parent
 *					component.
**/
void NaiveDevelopment::passDevelopmentSignal(CellComponent*component,
                                             float signal){
    CellComponent *parentComponent=mParent;
    if (component==parentComponent) {
        //assume it's a neuron
        mDevelopmentSignal+=signal;
    }else {
        Branch* b=dynamic_cast<Branch*>(component);//a little ugly we have to dynamic cast here
        //but I think that's the price we pay for unifying the class functions into a single glob here in the plugin
        assert(b);
        if(b->activityGreaterThanThreshold()){
            signal += b->signalWeight();
        }
        //recursion on parent component
        if (b->parent()){
            passDevelopmentSignal(b->parent(),signal);
        }
    }
}
///Simply route call to private overload
void NaiveDevelopment::passDevelopmentSignal(Synapse*s,
                                             CellComponent*sParent,
                                             float signalWeight){
    //FIXME: do we want to muck with  s->mConnectionStrength? That might be sensical based on some return value of recursion
    passDevelopmentSignal(sParent,signalWeight);
}

/**
 *	@param const ActivityStats& stats - activity statistics
 *
 *	Description:	If the neuron is in its early states then give it some initial strengthening or weakening.
 *					Otherwise, the connection is strengthened or weakened based on some other function (see code).
 *					If this synapse isn't connected to anything, then the above does not occur and the synapse is connected
 *					to a neuron based on the connect() function
**/



//FIXME have NaiveDevelopment read the following from the genes file: perhaps have some default that's modified by the genes present or absent in the region?
#define _STRENGTHEN_AMOUNT_		0.0f
#define _EARLY_DEV_WINDOW_		40
#define _INITIAL_STRENGTHEN_	0.04f
#define _INITIAL_WEAKEN_		-0.2f
#define _CHANGE_SIZE_			0.1f
#define _MAX_STRENGTHEN_		0.1f
#define _MAX_WEAKEN_			-0.04f
#define _STRENGTHEN_RANGE_		2.0f
#define _DISCONNECT_THRESHOLD_	0.3f
//ENDFIXME

bool initiated=initiateNaiveDevelopment();
void NaiveDevelopment::developSynapse(Synapse *s, const ActivityStats&stats){
	float strengthenAmount=_STRENGTHEN_AMOUNT_;
	int earlyDevelopmentWindow = _EARLY_DEV_WINDOW_;		//How many concurrent synapses firing is required to move to "mid-development"
	float initialStrengthen = _INITIAL_STRENGTHEN_;			//How much to strengthen a firing synapse in early development
	float initialWeaken = _INITIAL_WEAKEN_;
	float changeSize = _CHANGE_SIZE_;				//Controls how quickly strengthening and weakening happen in mid-development
	float maxStrengthen = _MAX_STRENGTHEN_;			//Maximum amount to strengthen in mid-development
	float maxWeaken = _MAX_WEAKEN_;					//Maximum amount to weaken in mid-development
	float strengthenRange = _STRENGTHEN_RANGE_;		//The multiplier to the level of signal that determines how much and whether to strengthen the synapse 

    if(s->recipient()){
    	if(stats.mBestDevelopmentSignal < earlyDevelopmentWindow){				//Neuron still in early state
	    	if(s->mFiringCounter > 0){							//If the synapse is active and not helping the neuron, weaken. If it is active and is helping, strengthen
	    		s->mConnectionStrength += initialStrengthen;					//Strengthen weakly in beginning
	    	}
	    	else{
	    		s->mConnectionStrength += initialWeaken;
				if(s->mConnectionStrength < _DISCONNECT_THRESHOLD_){
					s->detach();
				}
	    	}
	    }
	    else{
	    	if(s->mFiringCounter > 0){
	    		strengthenAmount = changeSize*(strengthenRange*stats.mDevelopmentSignal - stats.mBestDevelopmentSignal)/(stats.mDevelopmentSignal+0.001f);
	    		if(strengthenAmount > maxStrengthen){strengthenAmount = maxStrengthen;}
	    		if(strengthenAmount < maxWeaken){strengthenAmount = maxWeaken;}
	    		s->mConnectionStrength += strengthenAmount;
	    	}
	    	else{
	    		strengthenAmount = stats.mDevelopmentSignal/(10*stats.mBestDevelopmentSignal);
	    		if(strengthenAmount < maxWeaken){strengthenAmount = maxWeaken;}
	    		s->mConnectionStrength += strengthenAmount;
	    	}
	    }
	}
    else{
        s->connect();
    }
    
}
void NaiveDevelopment::mature(){
    mDevelopmentStage=1;
}

}
