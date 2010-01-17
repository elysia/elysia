
class CellComponent{
    float mActivity;                   
    float mThreshold;                  //How much activity is required to fire
    float mSignalStrength
    int mDevelopmentStage;                        //0 = development, 1=mature
public:
    void strengthen(float reward); //for punishment, use negative reward
    virtual ProteinDensity &getProteinDensityStructure()=0;
    virtual void activateComponent(float signal)=0; //negative possible for inhibition
};
