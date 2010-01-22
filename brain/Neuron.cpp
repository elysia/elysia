Neuron::Neuron(float BaseBranchiness, float TipBranchiness, int currentdepth, float TreeDepth){

    if(currentdepth < (TreeDepth/2){
        branchiness = BaseBranchiness;
     }
    else{branchiness = TipBranchiness;}
    branches = 0;
    while(branches < branchiness){
        if(branches - branchiness < 1){
            float a = rand(0,1);
            if(a < (branchiness - branches)){
                break;
                }
            }
        this->syncBranchDensity(BaseBranchiness, TipBranchiness, currentdepth + 1, TreeDepth);
        branches++;
        }
        
}
void Neuron::fire() {
    for (std::vector<Dendrite_Tip>::iterator i=attached_dendrites.begin(),ie=attached_dendrites.end();
         i!=ie;
	 ++i) {
      this->fire(*i);
}

ProteinDensity& Neuron::getProteinDensityStructure(){
    return mProteinDensityStructure;
}
void Neuron::growBranch() {
    Branch * b=new Branch(this);
    mChildBranches.push_back(b);
    
}
void Neuron::removeSynapse(Synapse*synapse){
  std::vector<Synapse* >::iterator where=std::find(mConnectedDendrites.begin(),mConnectedDendrites.end(),synapse);
  if (where!=mConnectedSynapses.end()) {
    mConnectedSynapses.erase(where);
  }else {
    std::cerr<< "Could not find synapse\n";
  }
}
void Neuron::tick(){
}