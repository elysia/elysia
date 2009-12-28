

int receptor_limit = 4;
int tree_depth = 4;
int target_limit = 1000;


class Lobe{
    public:
        int lobe_number;
        Neuron *child_neurons;
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

Neuron* grow_neuron(lobe_location, emitters, receptors, tree_branchiness, tree_thresholds, signal_duration, tree_pruning){
    Neuron *created_neuron = new Neuron(lobe_location, emitters, receptors, tree_branchiness, tree_thresholds, signal_duration, tree_pruning);
    return created_neuron;
}


//The neuron class receives activity from the dendrite class and then passes activity onto the dendrite tips that connect to it
class Neuron{
    public:
        static int target_limit;              //how many dendrites can connect to a single neuron
        static int total_neurons;
        int neuron_number;
        float neuron_location[2][2];          //The location of the neuron
        float cell_activity;
        float cell_threshold;
        Lobe *parent_lobe;
        Branch *child_branch[];                //Array of child branches
        int stage;                             //0 = development, 1=mature
        Dendrite_tip *target[100];             //100 is the limit for dendrite connections
        void fire(Dendrite_tip ->target);
        void grow_branch(Branch *child_branch);
        Neuron(int number, int threshold, int *lobe, float location);
        void detach_dendrite();
};

Neuron::Neuron(int threshold,Lobe *lobe,float location,int emitters,int receptors, tree_branchiness, tree_thresholds, signal_duration, tree_pruning){
    neuron_number = number;
    cell_threshold = threshold;
    parent_lobe = lobe;
    cell_location = location;
    cell_threshold = 0;
    cell_activity = 0;
    stage = 0; 
}

void Neuron::fire(->target){
    target -> activate();
}

void Neuron::detach_dendrite()


//The branch class covers all branch points in the dendrite tree. When a dendrite is active, it passes activity
//to the previous dendrite branch (whose identity it stores as a pointer). Neurons at the base 
class Branch{
    public:
        float activity;                   
        float threshold;                  //How much activity is required to fire
        int stage;                        //0 = development, 1=mature
        int signal_strength=1;
        Branch *upstream_branch;            //0 for a branch at the base, signifying to fire the neuron
        Neuron *parent_neuron;              //Which neuron does this dendrite belong to
        Dendrite_tip *child_dendrite_tips;  //Which dendrite tips come from this branch
        Branch *child_branch                //Pointers to branches that come off of this branch
        int depth;                          //How many levels into the dendrite tree this branch is
        int branches;                       //How many branches come immediately off of this point
        void set_activity(float excitation){activity += excitation;}
        void fire_branch(Branch ->upstream_branch);
        void fire_neuron(Neuron ->parent_neuron);
        void strengthen(Branch ->child_branch, Dendrite_tip ->child_dendrite_tip);
        void weaken(Branch ->child_branch, Dendrite_tip ->child_dendrite_tip);
};

void Branch::fire_branch(Branch ->upstream_branch, signal_strength){}

void Branch::fire_neuron(Neuron ->parent_neuron){}

void Branch::strengthen(Branch ->child_branch, Dendrite_tip ->child_dendrite_tip){}

void Branch::weaken(Branch ->child_branch, Dendrite_tip ->child_dendrite_tip){}



//The dendrite_tip represents connections from the dendrite tree onto other neurons. Neurons pass activity onto
//dendrite_tips and dendrite_tips pass that activity onto branches
class Dendrite_tip{
    public:
        float activity;                   
        float threshold;
        int stage;                                                                            //0 = development, 1=mature
        Branch *upstream_branch
        Neuron *recipient;
        void fire(Branch ->parent_dendrite, Dendrite_tip ->self);
        void connect_tip();
        void strengthen_tip();
        void weaken_tip();
        int detach(Neuron ->recipient, Dendrite_tip ->this);                                 //Detach dendrite tip from target neuron return 1 for success
        void activate(void);
};

int Dendrite_tip::detach(Neuron ->recipient, ->this){
    recipient -> detach_dendrite(->this);
    *recipient = 0;
}
    
void Dendrite_tip::fire(Dendrite ->parent_dendrite, Dendrite_tip ->self){}

void Dendrite_tip::strengthen_tip(){}

void Dendrite_tip::weaken_tip(){}

void Dendrite_tip::detach(){}

void Dendrite_tip::activate(void){
    activity = 1;
}

//Base development stage

//

void main(){
int Neuron::neuron_limit = 200;
Neuron neuron_list[];
Branch branch_list[];
Dendrite_tip tip_list[];
}









