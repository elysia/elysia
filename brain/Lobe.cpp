
Lobe::Lobe(number, density, home_coordinates, target_coordinates, emit, receive, tree_branch, tree_thresh, signal_dur, tree_prune){
    lobe_number = number;
    neuron_density = density;
    emitters = emit;
    receptors = receive;
    tree_branchiness = tree_branch;
    tree_thresholds = tree_thresh;
    signal_duration = signal_dur;
    tree_pruning = tree_prune;
    lobe_location = home_coordinates;
    lobe_neuron_count = 0;
    stage = 0;
    }