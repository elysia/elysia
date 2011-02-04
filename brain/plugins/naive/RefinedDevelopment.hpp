
#include "StandardDevelopment.hpp"
namespace Elysia {
class RefinedDevelopment: public StandardDevelopment<RefinedDevelopment> {
    float mDevelopmentSignal;
    float mBestDevelopmentSignal;
    void passDevelopmentSignal(CellComponent*component,
                               float signalWeight);
public:
    RefinedDevelopment();
    virtual void passDevelopmentSignal(Synapse*s,
                                       CellComponent*sParent,
                                       float signalWeight);
    void developSynapse(Synapse *s, const ActivityStats&stats);
    void mature();
    static bool initRefinedDevelopmentLibrary();
    static bool deinitRefinedDevelopmentLibrary();
};

}
