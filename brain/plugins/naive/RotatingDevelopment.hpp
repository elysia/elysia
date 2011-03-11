
#include "StandardDevelopment.hpp"
namespace Elysia {
class RotatingDevelopment: public StandardDevelopment<RotatingDevelopment> {
    float mDevelopmentSignal;
    float mBestDevelopmentSignal;
    void passDevelopmentSignal(CellComponent*component,
                               float signalWeight);
public:
    RotatingDevelopment();
    virtual void passDevelopmentSignal(Synapse*s,
                                       CellComponent*sParent,
                                       float signalWeight);
    void developSynapse(Synapse *s, const ActivityStats&stats);
    void mature();
    static bool initRotatingDevelopmentLibrary();
    static bool deinitRotatingDevelopmentLibrary();
};

}
