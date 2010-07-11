#include "ProteinEnvironment.hpp"
#include "genome.pb.h"
namespace Elysia {

/**
 * Class defines the protein soup that components interact with
 * The supporting functions use the genome to generate a list of zones used for defining neuron effects
 */
class BRAIN_CORE_EXPORT SimpleProteinEnvironment : public ProteinEnvironment{
  //Define the data that each zone carries with it
  struct BRAIN_CORE_EXPORT ProteinZone{
    //Define Gene-Soup Structure because each gene needs to associate to the correct soup contribution
    struct BRAIN_CORE_EXPORT GeneSoupStruct{
      //Soup activates genes so you need to know which soup is active
      typedef std::pair<Elysia::Genome::Effect,float> EffectAndDensityPair;
      typedef std::vector< EffectAndDensityPair > SoupVector;
      SoupVector mSoup;
      
      //This is the gene-related-to-the-soup
      Elysia::Genome::Gene mGenes;
    };
    std::vector< GeneSoupStruct > mGeneSoup;
    BoundingBox3f3f mBounds;
    const BoundingBox3f3f &getBoundingBox()const {return mBounds;}
    float getSpecificProteinDensity(Elysia::Genome::Effect); 
    ///"Complicated" function to update the soup for the next time iteration
    void updateSoupToNextSoup(const float age);
  };
  std::vector<ProteinZone> mMainZoneList;
  
private:
  ///Combine genes into a single-shared region
  ProteinZone combineZone(const SimpleProteinEnvironment::ProteinZone &a, const SimpleProteinEnvironment::ProteinZone&b , const BoundingBox3f3f &bbox);
  ///Relocate a zone to a new box location
  ProteinZone relocateZone(ProteinZone a, const BoundingBox3f3f &bbox);
  //Merge 2 zones of same gene type
  void combineZonesFromSameGene(ProteinZone &a, const ProteinZone&b);  
public:
  ///Initialize the main-zone-list by reading the genes and creating 1 zone per gene from Genome (and simplify)
  ProteinEnvironment& initialize(const Elysia::Genome::Genome&genes);
  ///Get protein density at a location (given location, and protein effect interested in)
  float getProteinDensity(const Vector3f &location, const Elysia::Genome::Effect&);
  ///Find all the proteins at a given point (given location)
  std::vector<std::pair<Elysia::Genome::Effect, float> > getCompleteProteinDensity(const Vector3f& location);
  ///Combine 2 zones by dividing it up into parts before reassembling them
  void chopZonePair(const ProteinZone &a, const ProteinZone &b, std::vector<ProteinZone> &output);
  ///Split and simplify large zone definitions into smaller component zones for calculations (given list of all zones)
  void zoneIntersection(std::vector<ProteinZone> mMainZoneList);
  ///Rebuild the zone list such that it exclude the 2 zones currently in question
  void rebuildZones(std::vector<ProteinZone>::const_iterator a, std::vector<ProteinZone>::const_iterator b, const std::vector<ProteinZone> &input, std::vector<ProteinZone> &output);

  ///Zone management functions to add and remove zones from the main list
  void addZones(const std::vector<ProteinZone> &mSubZoneList,
               std::vector<ProteinZone> &mMainZoneList);
  void removeZones(std::vector<ProteinZone> mSubZoneList,
                  std::vector<ProteinZone> mMainZoneList);

  ///Function to find the zone that a single point reside in
  ProteinZone &resideInZones(const Vector3f queryPoint, 
                             std::vector<ProteinZone> mMainZoneList);
  ///This function looks through the genes present in the location and their densities and selects the gene most likely to have contributed to the desored effects' density at the moment.
  virtual const Elysia::Genome::Gene& retrieveGene(const Vector3f &location, const Elysia::Genome::Effect&effect);

                             
  ~SimpleProteinEnvironment();
};
}

