#include "Platform.hpp"
#include "Placeable.hpp"
#include "AnnSpace.hpp"

#if 0
///placeholder class
class Placeable {
  Vector3f mPlaceableLocation;
public:
  const Vector3f& getLocation()const {return mPlaceableLocation;}
};
#endif

namespace Elysia {

	TreeNNSpatialSearch::TreeNNSpatialSearch(){
		pointUpperThreshold = 100;
		pointLowerThreshold = 2;
		root = new AnnSpace;
	}

	TreeNNSpatialSearch::~TreeNNSpatialSearch(){
		delete root;
	}
/**
 *	@param const Vector3f &queryPoint - point where to begin search
 *	@param Placeable *exclude - pointer to neuron which you don't want to return (i.e. itself)
 *	@returns nearest neighbor
 * 
 *	Description:	Finds nearest neighbor from local copy of list
**/
Placeable* TreeNNSpatialSearch::findNearestNeighbor(const Vector3f &queryPoint, Placeable* exclude){
    Placeable* target = root->findNN(queryPoint.x, queryPoint.y, exclude);
	return target;
}

/**
 *	@param Placeable *neuron - pointer to neuron to insert
 * 
 *	Description:	Function to *add* neighbor/point to local copy of list
**/
void TreeNNSpatialSearch::addNeighbor(Placeable* placeable){
    root->addPoint(placeable, this);
}

/**
 *	@param Placeable *neuron - pointer to neuron to remove
 * 
 *	Description:	Function to *remove* neighbor/point from local copy of list
**/
void TreeNNSpatialSearch::removeNeighbor(Placeable* placeable){
    root->deletePoint(placeable, this);
}

/**
 *	@param Placeable *neuron - pointer to neuron to remove
 * 
 *	Description:	Function to *update* neighbor/point to local copy of list
**/
void TreeNNSpatialSearch::moveNeighbor(Placeable* placeable){
	assert(false);
}
}
