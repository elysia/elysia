//Takes care of disconnecting DendriteTip. Should be called in destructor



void Synapse::detach(){
    mRecipient->removeSynapse(this);
    *mRecipient = NULL;
}

void Synapse::connect(){
    Neuron *parentneuron;
    parentneuron = mParentBranch.getParentNeuron();
    //Get the target bound from gene
    //Pick location from bound
    //Nearest neighbor target
    mConnectionStrength = 1;
    mRecipientNeuron.push_back(this);
}


bool pickrandomlocaton(Elysia::Genome::Gene gene, float age, Vector3f& retval){
    int numregions;
    numregions = gene.target_region_size();
    std::vector<BoundingBox3f3f> activeRegions;
    for(int i=0;i<numregions;i++){
        Elysia::Genome::TemporalBoundingBox boundingbox;
        boundingbox = gene.target_region(i);
        if((boundingbox.has_mint()==false||age>=boundingbox.mint()) && (boundingbox.has_maxt()==false||age<=boundingbox.maxt())) {
            activeregions.push_back(BoundingBox3f3f(Vector3f(boundingbox.minx(),boundingbox.miny(),boundingbox.has_minz()?boundingbox.minz():0.0),
                                                    Vector3f(boundingbox.maxx(),boundingbox.maxy(),boundingbox.has_maxz()?boundingbox.maxz():0.0)));
        }
        //active regions
        
    }
    if (activeRegions.empty()) {
      return false;
    }
    randomchoice = rand()%activeRegions.size()

    BoundingBox3f3f desiredregion = activeregions[randomchoice];
    xcoord = desiredregion.max() - desiredregion.min();
    a = rand()/RAND_MAX;
    
    
    
    

    
}
     
     