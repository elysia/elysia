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
/* function to find Nearest Neighbor from local copy of list
 */
Neuron* SimpleSpatialSearch::findNearestNeighbor(const Vector3f &queryPoint){
    std::tr1::unordered_set<Neuron*>::iterator i;
    float maxDistance;
    Neuron * maxDistanceItem=NULL;
    for(i=mNeurons.begin();i!=mNeurons.end();++i) {
       Neuron * current=*i;//find out what's "inside" i
       float currentDistance=(current->getLocation()-queryPoint).length();
       if (maxDistanceItem==NULL || currentDistance<maxDistance) {
            maxDistance=currentDistance;
            maxDistanceItem=current;
       }
    }
    return maxDistanceItem;
}

/* function to *add* neighbor/point to local copy of list
 */
void SimpleSpatialSearch::addNeighbor(Neuron* neuron){
    mNeurons.insert(neuron);
}


/* function to *remove* neighbor/point to local copy of list
 */
void SimpleSpatialSearch::removeNeighbor(Neuron* neuron){
    mNeurons.erase(neuron);
}


/* function to *update* neighbor/point to local copy of list
 */
void SimpleSpatialSearch::moveNeighbor(Neuron*){
}
}
