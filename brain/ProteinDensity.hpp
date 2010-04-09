#ifndef _PROTEIN_DENSITY_HPP_
#define _PROTEIN_DENSITY_HPP_

namespace Elysia {
class ProteinEnvironment;

class ProteinDensity {
    Elysia::Genome::Gene mGene;
    ProteinEnvironment *mExtrinsicProteins;
public:
  ProteinDensity(ProteinEnvironment*env, const Elysia::Genome::Gene&gene):mGene(gene){
        mExtrinsicProteins=env;
  }
  float getProteinDensity(const Elysia::Genome::Effect&);
  std::vector<std::pair<Elysia::Genome::Effect,float> > getCompleteProteinDensity();
  std::vector<BoundingBox3f3f> getTargetBounds(float age);
  Vector3f getRandomTargetPoint(float age);
};

}
#endif
