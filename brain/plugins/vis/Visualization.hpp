#include "BrainPlugin.hpp"
namespace Elysia {
class Brain;
class GraphicsSystem;
class Neuron;
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
    void drawNeuronBody(Neuron*n);
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
