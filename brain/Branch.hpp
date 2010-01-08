
//The branch class covers all branch points in the dendrite tree. When a dendrite is active, it passes activity
//to the previous dendrite branch (whose identity it stores as a pointer). Neurons at the base 
class Branch public CellComponent{
    public:
        int signal_strength;
        Branch *upstream_branch;            //0 for a branch at the base, signifying to fire the neuron
        Neuron *parent_neuron;              //Which neuron does this dendrite belong to
        Dendrite_tip *child_dendrite_tips;  //Which dendrite tips come from this branch
        Branch *child_branch                //Pointers to branches that come off of this branch
        int depth;                          //How many levels into the dendrite tree this branch is
        int branches;                       //How many branches come immediately off of this point
        void activate(float excitation);
        void fire_branch(Branch ->upstream_branch);
        void fire_neuron(Neuron ->parent_neuron);
        void strengthen(Branch ->child_branch, Dendrite_tip ->child_dendrite_tip, float reward); //for punishment, use negative reward
