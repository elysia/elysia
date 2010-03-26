#ifndef _PROTEIN_DENSITY_HPP_
#define _PROTEIN_DENSITY_HPP_

namespace Elysia {
class ProteinEnvironment;

class ProteinDensity {
    typedef std::map<Elysia::Genome::Effect,float> ProteinDensityMap;
    
    
    ProteinDensityMap mIntrinsicProteins;
    ProteinEnvironment *mExtrinsicProteins;
public:
  ProteinDensity(ProteinEnvironment*env){
        mExtrinsicProteins=env;
  }
  float getProteinDensity(const Elysia::Genome::Effect&);
  std::vector<ProteinDensityMap::value_type> getCompleteProteinDensity();
};

}
#endif
