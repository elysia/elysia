#include "SpatialSearch.hpp"
namespace Elysia {
class Placeable;

/** Find nearest neighbor from a scatter set of locations
 *  --"TreeNN" implementation of nearest neighbor
 */
class BRAIN_CORE_EXPORT TreeNNSpatialSearch : public SpatialSearch { 
    std::tr1::unordered_set<Placeable*> mPlaceables;
private:
public:
    Placeable* findNearestNeighbor(const Vector3f &queryPoint, Placeable* exclude);
    void addNeighbor(Placeable* neuron);
    void removeNeighbor(Placeable* neuron);
    void moveNeighbor(Placeable*);
	unsigned int pointUpperThreshold;			//How many points the space must contain before division
	unsigned int pointLowerThreshold;			//A delete event must take the space to this number for merging
	
	
};
}
