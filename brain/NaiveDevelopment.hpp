
#include "StandardDevelopment.hpp"
namespace Elysia {
class NaiveDevelopment: public StandardDevelopment<NaiveDevelopment> {
    float mDevelopmentSignal;
    void passDevelopmentSignal(CellComponent*component,
                               float signalWeight);
public:
    NaiveDevelopment();
    virtual void passDevelopmentSignal(Synapse*s,
                                       CellComponent*sParent,
                                       float signalWeight);
    void developSynapse(Synapse *s, const ActivityStats&stats);
    void mature();
};

}
