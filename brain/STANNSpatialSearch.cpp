#include "Platform.hpp"

#include "Placeable.hpp"
#include "STANNSpatialSearch.hpp"

#if 0
///placeholder class
class Placeable {
  Vector3f mPlaceableLocation;
public:
  const Vector3f& getLocation()const {return mPlaceableLocation;}
};
#endif

namespace Elysia {
/**
 *	@param const Vector3f &queryPoint - point where to begin search
 *	@param Placeable *exclude - pointer to neuron which you don't want to return (i.e. itself)
 *	@returns nearest neighbor
 * 
 *	Description:	Finds nearest neighbor from local copy of list
**/
Placeable* STANNSpatialSearch::findNearestNeighbor(const Vector3f &queryPoint, Placeable* exclude){
    std::tr1::unordered_set<Placeable*>::iterator i;
    float maxDistance;
    Placeable * maxDistanceItem=NULL;
    for(i=mPlaceables.begin();i!=mPlaceables.end();++i) {
       Placeable * current=*i;//find out what's "inside" i
       float currentDistance=(current->getLocation()-queryPoint).length();
       if ((maxDistanceItem==NULL || currentDistance<maxDistance) && current != exclude){
            maxDistance=currentDistance;
            maxDistanceItem=current;
       }
    }
    return maxDistanceItem;
}

/**
 *	@param Placeable *neuron - pointer to neuron to insert
 * 
 *	Description:	Function to *add* neighbor/point to local copy of list
**/
void STANNSpatialSearch::addNeighbor(Placeable* neuron){
    mPlaceables.insert(neuron);
}

/**
 *	@param Placeable *neuron - pointer to neuron to remove
 * 
 *	Description:	Function to *remove* neighbor/point from local copy of list
**/
void STANNSpatialSearch::removeNeighbor(Placeable* neuron){
    mPlaceables.erase(neuron);
}

/**
 *	@param Placeable *neuron - pointer to neuron to remove
 * 
 *	Description:	Function to *update* neighbor/point to local copy of list
**/
void STANNSpatialSearch::moveNeighbor(Placeable*){
}
}
