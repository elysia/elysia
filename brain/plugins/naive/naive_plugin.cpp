#include "../../Platform.hpp"
#include "NaiveDevelopment.hpp"
#include "RefinedDevelopment.hpp"

static int core_plugin_refcount = 0;

SIRIKATA_PLUGIN_EXPORT_C void init() {
    core_plugin_refcount++;
	Elysia::RefinedDevelopment::initRefinedDevelopmentLibrary();
    Elysia::NaiveDevelopment::initNaiveDevelopmentLibrary();
	
}

SIRIKATA_PLUGIN_EXPORT_C void destroy() {
	Elysia::RefinedDevelopment::deinitRefinedDevelopmentLibrary();
    Elysia::NaiveDevelopment::deinitNaiveDevelopmentLibrary();
    core_plugin_refcount--;
}

SIRIKATA_PLUGIN_EXPORT_C const char* name() {
    return "vis";
}

SIRIKATA_PLUGIN_EXPORT_C int refcount() {
    return core_plugin_refcount;
}
