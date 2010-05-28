
class BRAIN_CORE_EXPORT Lobe{
public:
    int lobe_number;
    std::vector<Neuron *>child_neuron;
    float lobe_location[2][2];                      //[0][0] lower left, [1][0] lower right, [0][1] upper left, [1][1] upper right
    float target_location[2][2];                    //[0][0] lower left, [1][0] lower right, [0][1] upper left, [1][1] upper right
    float neuron_density;                           //how many neurons does this lobe grow per unit area
    int stage;                                      //0 = development, 1 = mature
    int emitters[4];                                 //a list that determines which dendrites can synapse onto neurons from this lobe
    int receptors[4];                                //a list that determines which neurons can be targets of dendrites from this lobe
    int tree_branchiness[5];                         //used to build the neuron tree. An array that holds the number of branches at each level of the dendrite tree
    int tree_thresholds[5];                          //the number of branches that have to be active at each position in the branch to fire through. Neurons have their own value that they can adapt
    int signal_duration[5];                          //how long a branch in the tree holds onto signal
        float tree_pruning[5];                           //after building the tree, this proportion of branches are pruned
        Lobe (int number, float density, float home_coordinates, float target_coordinates int emit, int receive, int tree_branch, int tree_thresh, int signal_dur, int tree_prune);
        Neuron* grow_neuron(float lobe_location, int emitters, int receptors, int tree_branchiness, int tree_thresholds, int signal_duration, int tree_pruning);
        
        //to grow neuron, call the constructor for Neuron, with pointer back to lobe
        //to delete neuron, call destructor
        
};
