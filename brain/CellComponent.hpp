
class CellComponent{
    float activity;                   
    float threshold;                  //How much activity is required to fire
    int stage;                        //0 = development, 1=mature
public:
    void strengthen(float reward); //for punishment, use negative reward
    virtual ProteinDensity&getProteinDensityStructure();
};
