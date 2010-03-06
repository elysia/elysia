#include "SpatialSearch.hpp"
namespace Elysia {
class Neuron;


///placeholder class
class Neuron {
public:
  Vector3f mNeuronLocation;
};

/** Find nearest neighbor from a scatter set of locations
 *  --"Simple" implementation of nearest neighbor
 */
class SimpleSpatialSearch : public SpatialSearch { 
    std::tr1::unordered_set<Neuron*> mNeurons;
private:
public:
    Neuron* findNearestNeighbor(const Vector3f &queryPoint);
    void addNeighbor(Neuron* neuron);
    void removeNeighbor(Neuron* neuron);
    void updateNeighbor(Neuron*);
};
}
