#include "Platform.hpp"
#include "BrainPlugins.hpp"
namespace Elysia {
class BrainPlugin;
static BrainPlugins gBrainPlugins;
void BrainPlugins::registerBrainPlugin(const std::string&name, 
                                  const std::tr1::function<BrainPlugin*(Brain*)>&constructor){
    gBrainPlugins.mPlugins[name]=constructor;
}

std::vector<BrainPlugin *>BrainPlugins::constructAll(Brain*b) {
    std::vector<BrainPlugin*> retval;
    for (std::map<std::string,std::tr1::function<BrainPlugin*(Brain*)> >::iterator i=gBrainPlugins.mPlugins.begin(),ie=gBrainPlugins.mPlugins.end();i!=ie;++i) {
        retval.push_back(i->second(b));
    }
    return retval;
}
BrainPlugin *BrainPlugins::construct(const std::string&name, Brain*b) {
    std::vector<BrainPlugin*> retval;
    std::map<std::string,std::tr1::function<BrainPlugin*(Brain*)> >::iterator i=gBrainPlugins.mPlugins.find(name);
    if (i!=gBrainPlugins.mPlugins.end()){
        return i->second(b);
    }
    return NULL;
}

}
