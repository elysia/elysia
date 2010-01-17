class ProteinDeposit {
    Elysia::Genome::Gene mGene;
public:
    float currentDensity;
    Elysia::Genome::ProteinEffect getEffect();
    BoundingBox3f3f getBoundingBox();
    void update(float t, ProteinEnvironment*);
    
};
