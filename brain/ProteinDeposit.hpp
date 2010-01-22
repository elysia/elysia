class ProteinDeposit {
    std::vector<Elysia::Genome::Gene> mGenes;
    BoundingBox3f3f mBounds;
    std::vector<std::pair<Elysia::Genome::Effect,float> > mSoup;
public:
    float currentDensity;
    Elysia::Genome::ProteinEffect getEffect();
    BoundingBox3f3f getBoundingBox();
    void update(float t, ProteinEnvironment*);
    
};
