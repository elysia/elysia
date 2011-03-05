#include "Platform.hpp"
#include "Placeable.hpp"
#include "SpatialSearch.hpp"
#include "genome.pb.h"

#include "Brain.hpp"

namespace Elysia {
Placeable::Placeable(Brain * brain, const Vector3f&location):mNeuronLocation(location) {
    this->mBrain=brain;
    mBrain->getSpatialSearch()->addNeighbor(this);
}
Placeable::~Placeable() {
    mBrain->getSpatialSearch()->removeNeighbor(this);
    
}
}
