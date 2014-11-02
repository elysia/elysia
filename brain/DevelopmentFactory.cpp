#include "Platform.hpp"
#include "Development.hpp"
#include "DevelopmentFactory.hpp"
namespace Elysia {

DevelopmentFactory& DevelopmentFactory::getSingleton() {
    return AutoSingleton<DevelopmentFactory>::getSingleton();
}

void DevelopmentFactory::destroy() {
    AutoSingleton<DevelopmentFactory>::destroy();
}

}
