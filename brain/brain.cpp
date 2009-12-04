
int receptor_limit = 4;
int tree_depth = 4
int target_limit = 1000;
global int time=0;

class Lobe{
    public:
        float cell_location[2][2];                      //[0][0] lower left, [1][0] lower right, [0][1] upper left, [1][1] upper right
        float target_location[2][2];                    //[0][0] lower left, [1][0] lower right, [0][1] upper left, [1][1] upper right
        float neuron_density;                           //how many neurons does this lobe grow per unit area
        int emitters[receptor_limit];                   //a list that determines which dendrites can synapse onto neurons from this lobe
        int receptors[receptor_limit];                  //a list that determines which neurons can be targets of dendrites from this lobe
        int tree_branchiness[tree_depth];               //used to build the neuron tree. An array that holds the number of branches at each level of the dendrite tree
        int tree_thresholds[tree_depth];                //the number of branches that have to be active at each position in the branch to fire through. Neurons have their own value that they can adapt
        int signal_duration[tree_depth];                //how long a branch in the tree holds onto signal
        float tree_pruning[tree_depth];                 //after building the tree, this proportion of branches are pruned
        
        //to grow neuron, call the constructor for Neuron
        //to delete neuron, call destructor
        
}

//The neuron class receives activity from the dendrite class and then passes activity onto the dendrite tips that connect to it
class Neuron: public Lobe {
    public:
        float activity=0;
        float threshold=1;
        Dendrite_tip *target[target_limit];
        void fire(Dendrite_tip ->target);
};

void Neuron::fire(Dendrite_tip -> target){}


//The dendrite class covers all branch points in the dendrite tree. When a dendrite is active, it passes activity
//to the previous dendrite branch (whose identity it stores as a pointer). Neurons at the base 
class Branch: public Neuron{
    public:
        float activity=0;                   
        float threshold=1;                  //How much activity is required to fire
        Branch *upstream_branch;            //0 for a branch at the base, signifying to fire the neuron
        Neuron *parent_neuron;              //Which neuron does this dendrite belong to
        int depth;                          //How many levels into the dendrite tree this branch is
        int branches;                       //How many branches come immediately off of this point
        void fire_branch(Branch ->upstream_branch);
        void fire_neuron(Neuron ->parent_neuron);
};

void Branch::fire_branch(Branch ->upstream_branch){}
void Branch::fire_neuron(Neuron ->parent_neuron){}


//The dendrite_tip represents connections from the dendrite tree onto other neurons. Neurons pass activity onto
//dendrite_tips and dendrite_tips pass that activity onto dendrites
class Dendrite_tip public Branch{
    public:
        float activity=0;                   
        float threshold=1;
        Branch *upstream_branch
        Neuron *recipient;
        void fire(Dendrite ->parent_dendrite, Dendrite_tip ->self);
        void connect();
        int detach(Neuron ->recipient);                                 //Detach dendrite tip from target neuron return 1 for success
};

int Dendrite_tip::detach(Neuron ->target){
    
    return 1;
    else{
        return 0;
    }
}
    
void Dendrite_tip::fire(Dendrite ->parent_dendrite, Dendrite_tip ->self){}


}
















