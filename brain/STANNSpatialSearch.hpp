#include "SpatialSearch.hpp"
namespace Elysia {
class Neuron;

/** Find nearest neighbor from a scatter set of locations
 *  --"STANN" implementation of nearest neighbor
 */
class BRAIN_CORE_EXPORT STANNSpatialSearch : public SpatialSearch { 
    std::tr1::unordered_set<Neuron*> mNeurons;
private:
public:
    Neuron* findNearestNeighbor(const Vector3f &queryPoint, Neuron* exclude);
    void addNeighbor(Neuron* neuron);
    void removeNeighbor(Neuron* neuron);
    void moveNeighbor(Neuron*);
	unsigned int pointUpperThreshold;			//How many points the space must contain before division
	unsigned int pointLowerThreshold;			//A delete event must take the space to this number for merging
	
	
};
}
