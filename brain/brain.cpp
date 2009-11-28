
//The neuron class receives activity from the dendrite class and then passes activity onto the dendrite tips that connect to it
class Neuron{
    public:
        float activity=0;
        float threshold=1;
        float connection_range[2][2];                        //lower left and upper right corner where the dendrites look for input
        int receptor_type[receptor_limit];                   //determines which other neurons this neuron's dendrites can synapse onto
        int dendrite_tree[depth_limit][branchiness_limit][2] //determines the architecture of the tree. first value in 3rd D is the number of branches, the second is the probability that one is randomly pruned   
        int emitter_type[receptor_limit];                    //a list that determines which dendrites can synapse onto this neuron
        int *target[target_limit]=0;
        void grow_tree();
        void fire();
}

fire(){
    int i = 0;
    while(&targets[i] > 0){
        *target[i] = 1;
    }
}

//The dendrite class covers all branch points in the dendrite tree. When a dendrite is active, it passes activity
//to the previous dendrite branch (whose identity it stores as a pointer). Neurons at the base 
class Dendrite{
    public:
        float activity=0;                   
        float threshold=1;                  //How much activity is required to fire
        int depth;                          //How many levels into the dendrite tree this branch is
        int dendrite_tree[branchiness];     //How many branches come immediately off of this point
        int *parent;                        //Location of the branch this comes off of, or (if a branch at the base), the parent neuron
        void fire();
}

//The dendrite_tip represents connections from the dendrite tree onto other neurons. Neurons pass activity onto
//dendrite_tips and dendrite_tips pass that activity onto dendrites
class Dendrite_tip{
    public:
        float activity=0;                   
        float threshold=1;
        int *parent_dendrite;
        int *target;
        void fire();
        void connect();
}

connect(){
    
}



















// * gets the value stored at an address *var returns the value stored at the memory address var
// & gets the memory address &var returns the memory address var
