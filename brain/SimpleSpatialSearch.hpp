#include "SpatialSearch.hpp"
namespace Elysia {
class Placeable;

/** Find nearest neighbor from a scatter set of locations
 *  --"Simple" implementation of nearest neighbor
 */
class BRAIN_CORE_EXPORT SimpleSpatialSearch : public SpatialSearch { 
    std::tr1::unordered_set<Placeable*> mPlaceables;
private:
public:
    Placeable* findNearestNeighbor(const Vector3f &queryPoint, Placeable* exclude);
    void addNeighbor(Placeable* neuron);
    void removeNeighbor(Placeable* neuron);
    void moveNeighbor(Placeable*);
};
}
