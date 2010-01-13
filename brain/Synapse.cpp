//Takes care of disconnecting DendriteTip. Should be called in destructor
int Synapse::detach(->recipient, ->self){
    recipient -> detach_dendrite(self);
    *recipient = 0;
}
    
void Synapse::fire(->parent_branch, ->self){
    self -> set_acivity(0);
    parent_branch -> activate(1)
    
}

void Synapse::strengthen_tip(reward, ->self, ->recipient){
    connection_strength += reward;          //could use a more complicated reward function, this is rather naive
    if(connection_strength > 1){
        connection_strength = 1;
    }
    if(connection_strength <= 0){
        detach_dendrite(-> recipient, ->self);
    }
}

void Synapse::set_activity(float signal_strength){
    activity = signal_strength;
}
