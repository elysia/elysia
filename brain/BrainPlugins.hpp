#ifndef _LOADED_BRAIN_PLUGINS_HPP_
#define _LOADED_BRAIN_PLUGINS_HPP_

namespace Elysia{
class Brain;
class BrainPlugin;
class BRAIN_CORE_EXPORT BrainPlugins {
    std::map<std::string,std::tr1::function<BrainPlugin*(Brain*)> > mPlugins;
public:
    static void registerBrainPlugin(const std::string&name, const std::tr1::function<BrainPlugin*(Brain*)>&constructor);
    static BrainPlugin*construct(const std::string &pluginName, Brain*);
    static std::vector<BrainPlugin*>constructAll(Brain*);
};
}

#endif
