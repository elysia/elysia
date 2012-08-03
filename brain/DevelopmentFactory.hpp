

namespace Elysia {
class BRAIN_CORE_EXPORT DevelopmentFactory: public Sirikata::AutoSingleton<DevelopmentFactory>,
                                            public Sirikata::Factory<Development*> {
public:
    static DevelopmentFactory& getSingleton();
    static void destroy();
    
};

}
