

namespace Elysia {
class DevelopmentFactory: public AutoSingleton<DevelopmentFactory>,
                          public Factory<Development*> {
public:
    static DevelopmentFactory& getSingleton();
    static void destroy();
    
};

}
