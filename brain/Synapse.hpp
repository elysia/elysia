///The dendrite_tip represents connections from the dendrite tree onto other neurons. Neurons pass activity onto
///dendrite_tips and dendrite_tips pass that activity onto branches

class Synapse: public CellComponent{
public:
    std::vector<CellComponent *>parent_branch;
    std::vector<Neuron *>recipient;
    float connection_strength;
    //Detach dendrite tip from target neuron return 1 for success
    int detach(Neuron ->recipient, Synapse *self);
    void fire(Branch *parent_branch, Synapse *self);
    void connect_tip();
    void activate(void);
    void fire_branch(Branch ->upstream_branch);
    void strengthen_tip(float reward, Synapse ->self, Neuron ->recipient);
};