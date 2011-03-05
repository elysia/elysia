#include "Platform.hpp"
#include "Placeable.hpp"
#include "SpatialSearch.hpp"
#include "genome.pb.h"

#include "Brain.hpp"

namespace Elysia {
Placeable::Placeable(const Vector3f&location):mNeuronLocation(location) {
    mBrain->getSpatialSearch()->addNeighbor(this);
}
Placeable::~Placeable() {
    mBrain->getSpatialSearch()->removeNeighbor(this);
    
}
}
