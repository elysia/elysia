#include "Platform.hpp"
#include "BrainPlugins.hpp"
namespace Elysia {
class BrainPlugin;
static BrainPlugins gBrainPlugins;

/**
 *	@param const std::string &name - name of the plugin
 *	@param const std::function<BrainPlugin*(Brain*)> &constructor - brain constructor
 *
 *	Description:	Registers a (new) brain plugin
**/
void BrainPlugins::registerBrainPlugin(const std::string&name, 
                                  const std::function<BrainPlugin*(Brain*)>&constructor){
    gBrainPlugins.mPlugins[name]=constructor;
}

/**
 *	@param Brain *b - pointer to some brain for which you want to construct plugins
 *	@returns a vector of pointers to brain plugins
 *
 *	Description:	Constructs all plugins for this brain
**/
std::vector<BrainPlugin *>BrainPlugins::constructAll(Brain*b) {
    std::vector<BrainPlugin*> retval;
    for (std::map<std::string,std::function<BrainPlugin*(Brain*)> >::iterator i=gBrainPlugins.mPlugins.begin(),ie=gBrainPlugins.mPlugins.end();i!=ie;++i) {
        retval.push_back(i->second(b));
    }
    return retval;
}

/**
 *	@param const std::string &name - plugin name
 *	@param Brain *b - pointer to a brain object
 *	@returns a null pointer... why would you do this?
 *
 *	Description:	BrainPlugin constructor
**/
BrainPlugin *BrainPlugins::construct(const std::string &name, Brain *b) {
    std::vector<BrainPlugin*> retval;
    std::map<std::string,std::function<BrainPlugin*(Brain*)> >::iterator i=gBrainPlugins.mPlugins.find(name);
    if (i!=gBrainPlugins.mPlugins.end()){
        return i->second(b);
    }
    return NULL;
}

}
