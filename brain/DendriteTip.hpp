
//The dendrite_tip represents connections from the dendrite tree onto other neurons. Neurons pass activity onto
//dendrite_tips and dendrite_tips pass that activity onto branches
class DendriteTip public CellComponent{
    public:
        Branch *parent_branch
        Neuron *recipient;
        float connection_strength;
        void fire(Branch ->parent_branch, Dendrite_tip ->self);
        void connect_tip();
        int detach(Neuron ->recipient, Dendrite_tip ->self);                                 //Detach dendrite tip from target neuron return 1 for success
        void activate(void);
        void fire_branch(Branch ->upstream_branch);
        void strengthen_tip(float reward, Dendrite_tip ->self, Neuron ->recipient);
};