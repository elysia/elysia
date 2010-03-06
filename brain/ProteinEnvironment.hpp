#include "genome.pb.h"
namespace Elysia { namespace Genome {
 class Gene;
} }
/**
 * This class represents the bath of proteins that permeates individual cells on a regional basios
 * Cells can do point queries into this datastructure to see what the protein environment is 
 */
 namespace Elysia {
class ProteinEnvironment {
public:
  virtual ProteinEnvironment& initialize(const Elysia::Genome::Genome&)=0;
  ///This Function returns the protein density of a given protein effect in a given location. Classes can use this to find out how dense a particular protein is in a neuron area
  virtual float getProteinDensity(const Vector3f &location, const Elysia::Genome::Effect&effect)=0;
  ///This Function returns the entire list of proteins in an area. This is useful when a neuron needs to know a bunch of properties of the area and can search through the vector of returned effects and densities
  virtual std::vector<std::pair<Elysia::Genome::Effect, float> > getCompleteProteinDensity(const Vector3f& location)=0;
  virtual ~ProteinEnvironment(){}
};
}
