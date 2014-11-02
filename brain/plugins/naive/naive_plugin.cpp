#include "../../Platform.hpp"
#include "NaiveDevelopment.hpp"
#include "RefinedDevelopment.hpp"
#include "RotatingDevelopment.hpp"

static int core_plugin_refcount = 0;

ELYSIA_PLUGIN_EXPORT_C void init() {
    core_plugin_refcount++;
	Elysia::RotatingDevelopment::initRotatingDevelopmentLibrary();
	Elysia::RefinedDevelopment::initRefinedDevelopmentLibrary();
    Elysia::NaiveDevelopment::initNaiveDevelopmentLibrary();
	
}

ELYSIA_PLUGIN_EXPORT_C void destroy() {
	Elysia::RotatingDevelopment::deinitRotatingDevelopmentLibrary();
	Elysia::RefinedDevelopment::deinitRefinedDevelopmentLibrary();
    Elysia::NaiveDevelopment::deinitNaiveDevelopmentLibrary();
    core_plugin_refcount--;
}

ELYSIA_PLUGIN_EXPORT_C const char* name() {
    return "vis";
}

ELYSIA_PLUGIN_EXPORT_C int refcount() {
    return core_plugin_refcount;
}
