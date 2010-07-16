#include "Platform.hpp"

#include "Neuron.hpp"
#include "SimpleSpatialSearch.hpp"

#if 0
///placeholder class
class Neuron {
  Vector3f mNeuronLocation;
public:
  const Vector3f& getLocation()const {return mNeuronLocation;}
};
#endif

namespace Elysia {
/**
 *	@param const Vector3f &queryPoint - point where to begin search
 *	@param Neuron *exclude - pointer to neuron which you don't want to return (i.e. itself)
 *	@returns nearest neighbor
 * 
 *	Description:	Finds nearest neighbor from local copy of list
**/
Neuron* SimpleSpatialSearch::findNearestNeighbor(const Vector3f &queryPoint, Neuron* exclude){
    std::tr1::unordered_set<Neuron*>::iterator i;
    float maxDistance;
    Neuron * maxDistanceItem=NULL;
    for(i=mNeurons.begin();i!=mNeurons.end();++i) {
       Neuron * current=*i;//find out what's "inside" i
       float currentDistance=(current->getLocation()-queryPoint).length();
       if ((maxDistanceItem==NULL || currentDistance<maxDistance) && current != exclude){
            maxDistance=currentDistance;
            maxDistanceItem=current;
       }
    }
    return maxDistanceItem;
}

/**
 *	@param Neuron *neuron - pointer to neuron to insert
 * 
 *	Description:	Function to *add* neighbor/point to local copy of list
**/
void SimpleSpatialSearch::addNeighbor(Neuron* neuron){
    mNeurons.insert(neuron);
}

/**
 *	@param Neuron *neuron - pointer to neuron to remove
 * 
 *	Description:	Function to *remove* neighbor/point from local copy of list
**/
void SimpleSpatialSearch::removeNeighbor(Neuron* neuron){
    mNeurons.erase(neuron);
}

/**
 *	@param Neuron *neuron - pointer to neuron to remove
 * 
 *	Description:	Function to *update* neighbor/point to local copy of list
**/
void SimpleSpatialSearch::moveNeighbor(Neuron*){
}
}
