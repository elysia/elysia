namespace Elysia { namespace Genome {
 class Gene;
} }
/**
 * This class represents the bath of proteins that permeates individual cells on a regional basios
 * Cells can do point queries into this datastructure to see what the protein environment is 
 */
class ProteinEnvironment {
public:
  float getProteinDensity(const Vector3 &location, const Elysia::Genome::ProteinEffect&)=0;
  std::vector<std::pair<Elysia::Genome::ProteinEffect, float> > getCompleteProteinDensity(const Vector3& location)=0;
};