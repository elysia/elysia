
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

virtual void Branch::growBranch(EarlyBranchiness, LateBranchiness, currentdepth, TreeDepth){
    Branch * b=new Branch(this, EarlyBranchiness, LateBranchiness, currentdepth, TreeDepth);
    mChildBranches.push_back(b);


