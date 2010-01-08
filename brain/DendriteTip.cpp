
int DendriteTip::detach(->recipient, ->self){
    recipient -> detach_dendrite(self);
    *recipient = 0;
}
    
void DendriteTip::fire(->parent_branch, ->self){
    self -> set_acivity(0);
    parent_branch -> activate(1)
    
}

void DendriteTip::strengthen_tip(reward, ->self, ->recipient){
    connection_strength += reward;          //could use a more complicated reward function, this is rather naive
    if(connection_strength > 1){
        connection_strength = 1;
    }
    if(connection_strength <= 0){
        detach_dendrite(-> recipient, ->self);
    }
}

void DendriteTip::set_activity(float signal_strength){
    activity = signal_strength;
}
