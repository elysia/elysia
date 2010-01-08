
void Branch::fire_branch(Branch ->upstream_branch, signal_strength){
    upstream_branch -> set_activity(1.00);
}

void Branch::fire_neuron(Neuron ->parent_neuron){
    parent_neuron -> set_activity(1.00);
}

void Branch::strengthen(->child_branch, ->child_dendrite_tip, reward){
    if(child_dendrite_tip > 0){
        child_dendrite_tip -> strenghen_tip(reward);
    }
    child_branch -> strengthen(reward);
}

void Branch::activate(excitation){
    activity += excitation
    if(activity ) threshold){
        upstream_branch -> fire_branch;
    }
}


