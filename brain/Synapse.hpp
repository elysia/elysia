///The dendrite_tip represents connections from the dendrite tree onto other neurons. Neurons pass activity onto
///dendrite_tips and dendrite_tips pass that activity onto branches

class Synapse{
public:
    CellComponent* mParentBranch;
    Neuron *mRecipientNeuron;
    float mConnectionStrength;
    //Detach dendrite tip from target neuron return 1 for success
    int detach();
    void connect();     //need to get the location bound from neuron
};