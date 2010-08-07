#ifndef PROTEIN_ENVIRONMENT_HPP
#define PROTEIN_ENVIRONMENT_HPP

#include "genome.pb.h"
namespace Elysia { namespace Genome {
 class Gene;
} }
/**
 * This class represents the bath of proteins that permeates individual cells on a regional basios
 * Cells can do point queries into this datastructure to see what the protein environment is 
 */
 namespace Elysia {
class BRAIN_CORE_EXPORT ProteinEnvironment {
public:
    class iterator{
        ProteinEnvironment *parent;
    public:
        int which;
        iterator operator++(int) {
            iterator retval=*this;
            parent->increment(this);
            return retval;
        }
        iterator& operator++() {
            parent->increment(this);
            return *this;
        }
        bool operator ==(const iterator&other) const{
            return other.which==which&&other.parent==parent;
        }
        bool operator !=(const iterator&other) const{
            return !(*this==other);
        }
        size_t uid() const {
            return which;
        }
        BoundingBox3f3f getBoundingBox() const {
            return parent->getBoundingBox(*this);
        }
        float getProteinDensity(const Elysia::Genome::Effect&e) {
            return parent->getProteinDensity(*this,e);
        }
        ///Find all the proteins at a given point (given location)
        std::vector<std::pair<Elysia::Genome::Effect, float> > getCompleteProteinDensity() {
            return parent->getCompleteProteinDensity(*this);
        }
        const Elysia::Genome::Gene& retrieveGene(const Elysia::Genome::Effect&effect){
            return parent->retrieveGene(*this,effect);
        }
    };
    
  ///the first uniform protein zone in a reasonable ordering of zones
  virtual iterator begin()const=0;
  ///just past the last uniform protein zone in a reasonable ordering of zones
  virtual iterator end()const=0;
  ///increment an iterator (usually incrementing its count or offset) to point to the next zone so all protein zones are hit from the path of begin to end (not inclusive)
  virtual void increment(iterator*)const=0;
  ///get the bounding box of a given unform region
  virtual BoundingBox3f3f getBoundingBox(iterator it)const=0;
  ///This initialization function reads the genome information and creates the zones that sets up the protein environment
  virtual ProteinEnvironment& initialize(const Elysia::Genome::Genome&)=0;
  ///This Function returns the protein density of a given protein effect in a given location. Classes can use this to find out how dense a particular protein is in a neuron area
  virtual float getProteinDensity(const Vector3f &location, const Elysia::Genome::Effect&effect)=0;
  ///This Function returns the protein density of a given protein effect in a given location. Classes can use this to find out how dense a particular protein is in a neuron area
  virtual float getProteinDensity(iterator it, const Elysia::Genome::Effect&)=0;
  ///This function looks through the genes present in the location and their densities and selects the gene most likely to have contributed to the desored effects' density at the moment.
  virtual const Elysia::Genome::Gene& retrieveGene(const Vector3f &location, const Elysia::Genome::Effect&effect)=0;
  /// returns an iterator to the location specified by the vector3f
  virtual ProteinEnvironment::iterator getIterator(const Vector3f& where)=0;
  ///This function looks through the genes present in the location and their densities and selects the gene most likely to have contributed to the desored effects' density at the moment.
  virtual const Elysia::Genome::Gene& retrieveGene(iterator location, const Elysia::Genome::Effect&effect)=0;
  ///This function returns the entire list (repetitions allowed) of proteins in an area. This is useful when a neuron needs to know a bunch of properties of the area and can search through the vector of returned effects and densities
  virtual std::vector<std::pair<Elysia::Genome::Effect, float> > getCompleteProteinDensity(const Vector3f& location)=0;
  ///This function returns the entire list (repetitions allowed) of proteins in an area. This is useful when a neuron needs to know a bunch of properties of the area and can search through the vector of returned effects and densities
  virtual std::vector<std::pair<Elysia::Genome::Effect, float> > getCompleteProteinDensity(iterator it)=0;
  virtual BoundingBox3f3f getBounds()const=0;
  virtual ~ProteinEnvironment(){}
};
}

 #endif
