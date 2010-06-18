#ifndef _BRAIN_PLUGIN_HPP_
#define _BRAIN_PLUGIN_HPP_
namespace Elysia {
class Brain;

class BRAIN_CORE_EXPORT BrainPlugin {
public:
    BrainPlugin();
    virtual void initialize(Brain*b)=0;
    virtual void update()=0;//called each frame
    virtual ~BrainPlugin();
};

}
#endif
