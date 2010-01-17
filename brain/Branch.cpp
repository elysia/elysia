
Branch::Branch(CellComponent * parent) {
    mParentComponent=parent;
}

ProteinDensity&Branch::getProteinDensityStructure() {
    return mParentComponent->getProteinDensityStructure();
}

void Branch::growSynapse(){
    synapse *b = new Synapse(this);
    mChildSynapse.push_back(b);
    }


