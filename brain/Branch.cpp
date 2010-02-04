#include "Platform.hpp"
#include "Branch.hpp"
#include "Synapse.hpp"
namespace Elysia {
Branch::Branch(CellComponent * parent) {
    mParentComponent=parent;
}

ProteinDensity&Branch::getProteinDensityStructure() {
    return mParentComponent->getProteinDensityStructure();
}

/* do we use this yet? it needs to attach itself and initialize as well
void Branch::growSynapse(){
    Synapse *b = new Synapse(this);
    mChildSynapse.push_back(b);
}
*/
}
