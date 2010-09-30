#ifndef _STANDARD_DEVELOPMENT_HPP_
#define _STANDARD_DEVELOPMENT_HPP_
#include "Development.hpp"
#include "Neuron.hpp"
#include "Branch.hpp"
#include "Synapse.hpp"
namespace Elysia {
template<class T> class StandardDevelopment : public Development {
protected:
    ///Counts down to 0 to trigger next developmental re-evaluation
    int mDevelopmentCounter;
    ///What stage of development the neuron is in 0 for Developing, 1 for Mature
    int mDevelopmentStage;
public:

    StandardDevelopment() {
        mDevelopmentCounter=0;
        mDevelopmentStage=0;
    }


/**
 *	@param const ActivityStats& stats - activity statistics for this synapse
 *
 *	Description:	For all the child synapses of this branch, call their develop() methods. 
 *					Then call this function for all child synapses, using the argument passed
 *					to this function as the argument for the above two.
**/
    void developBranch(Branch*b,const ActivityStats&stats) {
        for (std::vector<Synapse*>::iterator i=b->childSynapseBegin(),ie=b->childSynapseEnd();
             i!=ie;
             ++i) {
            static_cast<T*>(this)->developSynapse(*i,stats);
        }
        for (Branch::ChildIterator i=b->childBegin(),ie=b->childEnd();
             i!=ie;
             ++i) {
            developBranch(*i,stats);
        }
    }
/**
 *	@param const ActivityStats& stats - activity statistics for this synapse
 *
 *	Description:	For all the child synapses of this neuron, call their develop() methods. 
**/
    void developNeuron(const ActivityStats&stats) {
        for (Neuron::ChildIterator i=mParent->childBegin(),ie=mParent->childEnd();
             i!=ie;
             ++i) {
            developBranch(*i,stats);
        }
    }
    void develop() {
        if(mDevelopmentStage == 0){
            if(mDevelopmentCounter == 0){
                developNeuron(mParent->getActivityStats());
                //mDevelopmentCounter	= 30;					//number of timesteps before next development re-evaluation
            }
            else{ mDevelopmentCounter--;}
        }
    }
    void matureNeuron(){
        static_cast<T*>(this)->mature();
        mDevelopmentStage = 1;
         
    }

};
}
#endif
