#include "BrainPlugin.hpp"
namespace Elysia {
class Brain;
class CellComponent;
class GraphicsSystem;
class Neuron;
class Branch;
class Visualization:public BrainPlugin  {
    std::tr1::shared_ptr<GraphicsSystem> mGraphics;
    static std::tr1::weak_ptr<GraphicsSystem> mGlobalGraphics;
    Brain * mBrain;
    float mNeuronSize;
    float mScale;
    Vector3f mOffset;
    bool mSynapseSnapToEdge;
    std::tr1::unordered_set<Neuron*> mSelected;
    Vector3f getNeuronLocation(Neuron*n)const;
    bool getNeuronWidthHeight(const std::string&text, float&width, float&hei, bool selected);
    void getSynapseStartEnd(Neuron * start, bool startIsSelected, Neuron * end, bool endIsSelected, Vector3f& A, Vector3f &B);
    ///Returns top location from where dendrites can start branching out
    Vector3f drawNeuronBody(Neuron*n);
    void drawBranch(const Neuron * n, const Branch* dendrite, Vector3f top, float scale) ;
    void drawDendrites(const Neuron* n, const CellComponent* dendrite, Vector3f startLocation, float scale);
    void drawNeuron(Neuron*n);
    void doInput();
    
public:
    Visualization();
    void draw();
    void update();
    void initialize(Brain*b);
    ~Visualization();
};
BrainPlugin* makeVisualization(Brain*b);
}
