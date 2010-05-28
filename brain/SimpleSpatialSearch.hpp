#include "SpatialSearch.hpp"
namespace Elysia {
class Neuron;

/** Find nearest neighbor from a scatter set of locations
 *  --"Simple" implementation of nearest neighbor
 */
class BRAIN_CORE_EXPORT SimpleSpatialSearch : public SpatialSearch { 
    std::tr1::unordered_set<Neuron*> mNeurons;
private:
public:
    Neuron* findNearestNeighbor(const Vector3f &queryPoint);
    void addNeighbor(Neuron* neuron);
    void removeNeighbor(Neuron* neuron);
    void moveNeighbor(Neuron*);
};
}
