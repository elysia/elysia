#ifndef _ELYSIA_BRANCH_HPP_
#define _ELYSIA_BRANCH_HPP_
#include "CellComponent.hpp"
#include <vector>
namespace Elysia {
class Synapse;
class Branch;
//The branch class covers all branch points in the dendrite tree. When a dendrite is active, it passes activity
//to the previous dendrite branch (whose identity it stores as a pointer). Neurons at the base 
class BRAIN_CORE_EXPORT Branch:  public CellComponent{

    CellComponent *mParentComponent;
    //Which branch does this synapse belong to
    std::vector<Synapse *>mChildSynapses;
    //Which dendrite tips come from this branch
    float mSignalWeight;
    
   public:
   ~Branch();
   virtual Neuron*getParentNeuron(){return mParentComponent->getParentNeuron();}

    Branch(CellComponent*parent);
    virtual ProteinDensity&getProteinDensityStructure();
    //Pointers to branches that come off of this branch
    void growSynapse();
    void activateComponent(Brain&,float signal);
    float signalWeight() const{
        return mSignalWeight;
    }	
	void visualizeTree(FILE *dendriteTree, size_t parent);
    typedef std::vector<Synapse*>::iterator SynapseIterator;
    typedef std::vector<Synapse*>::const_iterator SynapseConstIterator;
    SynapseIterator childSynapseBegin(){ return mChildSynapses.begin();}
    SynapseIterator childSynapseEnd(){ return mChildSynapses.end();}
    SynapseConstIterator childSynapseBegin()const{ return mChildSynapses.begin();}
    SynapseConstIterator childSynapseEnd()const{ return mChildSynapses.end();}
	float developmentWeight;
    CellComponent *parent() {
        return mParentComponent;
    }
};

}
#endif
