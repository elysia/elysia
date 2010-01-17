#include <vector>

///The neuron class receives activity from the dendrite class and then passes activity onto the dendrite tips that connect to it
class Neuron public CellComponent{
private:
    ///Neuron location
    Vector3f neuron_location;
    Lobe *parent_lobe;
    //Array of child branches
    std::vector<Branch *>child_branches;
    //Array of dendrites connections
    std::vector<Dendrite_tip *>connected_dendrites;
    /**
     * Fire activates a DendriteTip
     * \param target is the DendriteTip that should be activated
     * \returns whether the signal successfully passed through the inter-neuron gap
     */
    bool fire(DendriteTip *target);
public:
    Neuron(int number, int threshold, Lobe *lobe/*parent?*/, Vector3f location);
    void fire();
    void growBranch();
    ///Simulates one millisecond of neural time
    void tick();
    ProteinDensity& getProteindensityStructure();
};
