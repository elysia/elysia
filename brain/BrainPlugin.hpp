#ifndef _BRAIN_PLUGIN_HPP_
#define _BRAIN_PLUGIN_HPP_
namespace Elysia {
class Brain;
class Neuron;
class BRAIN_CORE_EXPORT BrainPlugin {
public:
    BrainPlugin();
    virtual void initialize(Brain*b)=0;
    virtual void update()=0;//called each frame
    ///called just before a neuron is destructed, so that the plugin can remove references to it
    virtual void notifyNeuronDestruction(Neuron* n)=0;
    virtual ~BrainPlugin();
};

}
#endif
