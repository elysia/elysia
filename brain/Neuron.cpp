
Neuron::Neuron(int threshold,Lobe *lobe,float location,int emitters,int receptors, tree_branchiness, tree_thresholds, signal_duration, tree_pruning){
    neuron_number = number;
    cell_threshold = threshold;
    parent_lobe = lobe;
    cell_location = location;
}
void Neuron::fire() {
    for (std::vector<Dendrite_Tip>::iterator i=attached_dendrites.begin(),ie=attached_dendrites.end();
         i!=ie;
	 ++i) {
      this->fire(*i);
}
void Neuron::fire(->target){
    target -> activate();
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
  if (where!=mConnectedDendrites.end()) {
    mConnectedDendrites.erase(where);
  }else {
    std::cerr<< "Could not find synapse\n";
  }
}