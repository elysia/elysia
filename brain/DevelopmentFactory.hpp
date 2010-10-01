

namespace Elysia {
class BRAIN_CORE_EXPORT DevelopmentFactory: public AutoSingleton<DevelopmentFactory>,
                          public Factory<Development*> {
public:
    static DevelopmentFactory& getSingleton();
    static void destroy();
    
};

}
