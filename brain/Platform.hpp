#ifndef _BRAIN_CORE_PLATFORM_HPP_
#define _BRAIN_CORE_PLATFORM_HPP_
#include <sirikata/util/Platform.hpp>

#ifndef BRAIN_CORE_EXPORT

# if SIRIKATA_PLATFORM == PLATFORM_WINDOWS
#   if defined(STATIC_LINKED)
#     define BRAIN_CORE_EXPORT
#   else
#     if defined(BRAIN_CORE_BUILD)
#       define BRAIN_CORE_EXPORT __declspec(dllexport)
#     else
#       define BRAIN_CORE_EXPORT __declspec(dllimport)
#     endif
#   endif
#   define BRAIN_CORE_PLUGIN_EXPORT __declspec(dllexport)
# else
#   if defined(__GNUC__) && __GNUC__ >= 4
#     define BRAIN_CORE_EXPORT __attribute__ ((visibility("default")))
#     define BRAIN_CORE_PLUGIN_EXPORT __attribute__ ((visibility("default")))
#   else
#     define BRAIN_CORE_EXPORT
#     define BRAIN_CORE_PLUGIN_EXPORT
#   endif
# endif
#endif
namespace Elysia {
using namespace Sirikata;
class SimTime {
    Sirikata::int64 time;
public:
    SimTime() {
        time=0;
    }
    bool operator !=(const SimTime&other) const{
        return other.time!=time;
    }
    bool operator ==(const SimTime&other) const{
        return other.time==time;
    }
    SimTime& operator ++() {
        ++time;
        return *this;
    }
};
}
#endif
