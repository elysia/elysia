#include <sirikata/util/Platform.hpp>

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
