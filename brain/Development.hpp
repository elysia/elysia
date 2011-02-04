#ifndef _DEVELOPMENT_HPP_
#define _DEVELOPMENT_HPP_
namespace Elysia {
class Neuron;
class Synapse;
class CellComponent;
class Branch;
class Development {
protected:
    Neuron * mParent;
public:
    virtual void develop()=0;
    virtual void passDevelopmentSignal(Synapse*s,
                                       CellComponent*sParent,
                                       float signalWeight)=0;
    virtual void setParent(Neuron*n){
        mParent=n;
    }
};


}
#endif
