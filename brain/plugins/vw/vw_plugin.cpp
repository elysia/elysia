#include "../../Platform.hpp"
#include "BrainPlugins.hpp"
#include "BrainObjectScriptManager.hpp"
#include <sirikata/oh/ObjectScriptManagerFactory.hpp>
#include <iostream>
static int core_plugin_refcount = 0;

SIRIKATA_PLUGIN_EXPORT_C void init() {
   using namespace Sirikata;
   using std::tr1::placeholders::_1;
   if (core_plugin_refcount == 0) {
       ObjectScriptManagerFactory::getSingleton().registerConstructor(
           "vw",
           std::tr1::bind(
               &Elysia::BrainObjectScriptManager::createObjectScriptManager,
               _1
               )
           );
       std::cout<<"INITIALIZE BRAIN PLUGIN\n";

   }
   core_plugin_refcount++;
   //Elysia::BrainPlugins::registerBrainPlugin("VW",&Elysia::makeVisualization);
}

SIRIKATA_PLUGIN_EXPORT_C void destroy() {
    std::cout<<"UNINITIALIZE BRAIN PLUGIN\n";
    //Elysia::Deinitialize();
    core_plugin_refcount--;
}

SIRIKATA_PLUGIN_EXPORT_C const char* name() {
    return "vw";
}

SIRIKATA_PLUGIN_EXPORT_C int refcount() {
    return core_plugin_refcount;
}
SIRIKATA_PLUGIN_EXPORT_C int increfcount() {
    return ++core_plugin_refcount;
}
SIRIKATA_PLUGIN_EXPORT_C int decrefcount() {
    assert(core_plugin_refcount);
    return --core_plugin_refcount;
}
