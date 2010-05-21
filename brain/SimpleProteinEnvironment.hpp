#include "ProteinEnvironment.hpp"
#include "genome.pb.h"
namespace Elysia {

/** Class defines the protein soup that components interact with
 */
class SimpleProteinEnvironment : public ProteinEnvironment{
  //Define the data that each zone carries with it
  struct ProteinZone{
    typedef std::pair<Elysia::Genome::Effect,float> EffectAndDensityPair;
    //Soup activates genes so you need to know which soup is active
    std::vector< EffectAndDensityPair > mSoup;
    std::vector< Elysia::Genome::Gene > mGenes;
    BoundingBox3f3f mBounds;
    const BoundingBox3f3f &getBoundingBox()const {return mBounds;}
    float getSpecificProteinDensity(Elysia::Genome::Effect); 
    //"Complicated" function to update the soup for the next time iteration
    void updateSoupToNextSoup(const float age);
  };
  std::vector<ProteinZone> mMainZoneList;
  
private:
  ProteinZone combineZone(const SimpleProteinEnvironment::ProteinZone &a, const SimpleProteinEnvironment::ProteinZone&b , const BoundingBox3f3f &bbox);
  ProteinZone relocateZone(ProteinZone a, const BoundingBox3f3f &bbox);
  void combineZonesFromSameGene(ProteinZone &a, const ProteinZone&b);  
public:
  ProteinEnvironment& initialize(const Elysia::Genome::Genome&genes);
  float getProteinDensity(const Vector3f &location, const Elysia::Genome::Effect&);
  std::vector<std::pair<Elysia::Genome::Effect, float> > getCompleteProteinDensity(const Vector3f& location);
  void chopZonePair(const ProteinZone &a, const ProteinZone &b, std::vector<ProteinZone> &output);

  //Split large zone definitions into smaller component zones for calculations
  void zoneIntersection(std::vector<ProteinZone> mMainZoneList);

  void rebuildZones(std::vector<ProteinZone>::const_iterator a, std::vector<ProteinZone>::const_iterator b, const std::vector<ProteinZone> &input, std::vector<ProteinZone> &output);

  //Zone management functions to add and remove zones from the main list
  void addZones(const std::vector<ProteinZone> &mSubZoneList,
               std::vector<ProteinZone> &mMainZoneList);
  void removeZones(std::vector<ProteinZone> mSubZoneList,
                  std::vector<ProteinZone> mMainZoneList);

  //Function to find the zone that a single point reside in
  ProteinZone &resideInZones(const Vector3f queryPoint, 
                             std::vector<ProteinZone> mMainZoneList);
  ///This function looks through the genes present in the location and their densities and selects the gene most likely to have contributed to the desored effects' density at the moment.
  virtual const Elysia::Genome::Gene& retrieveGene(const Vector3f &location, const Elysia::Genome::Effect&effect);

                             
  ~SimpleProteinEnvironment();
};
}

