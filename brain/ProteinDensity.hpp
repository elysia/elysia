namespace Elysia {
class ProteinEnvironment;

class ProteinDensity {
    typedef std::map<Elysia::Genome::Effect,float> ProteinDensityMap;
    
    
    ProteinDensityMap mIntrinsicProteins;
    ProteinEnvironment *mExtrinsicProteins;
public:
  float getProteinDensity(const Elysia::Genome::Effect&);
  std::vector<ProteinDensityMap::value_type> getCompleteProteinDensity();
};

}
