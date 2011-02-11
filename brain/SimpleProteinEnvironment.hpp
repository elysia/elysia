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
      typedef std::vector< EffectAndTypeAndDensity > SoupVector;
      SoupVector mSoup;
      
      //This is the gene-related-to-the-soup
      Elysia::Genome::Gene mGenes;
    };
    std::vector< GeneSoupStruct > mGeneSoup;
    BoundingBox3f3f mBounds;
    const BoundingBox3f3f &getBoundingBox()const {return mBounds;}
    float getSpecificProteinDensity(Elysia::Genome::Effect); 
    ///"Complicated" function to update the soup for the next time iteration
    void updateEachZoneGeneSoup(std::vector<ProteinZone::GeneSoupStruct> &mygenesoup, float age);
    bool isGeneOn(const Genome::Gene &currentgene);
    bool isConditionClauseTrue(const Genome::ConditionClause &currentClause);
    float getSpecificProteinDensity(const ProteinType &myProtein);
    bool isConditionTrue(const Genome::Condition &currentCondition);
    void driveGeneDensity(ProteinZone::GeneSoupStruct &targetGeneSoup, bool isGeneActive);
  };
  std::vector<ProteinZone> mMainZoneList;
  //Need outter function to handle update looping
  
private:
  ///Combine genes into a single-shared region
  ProteinZone combineZone(const SimpleProteinEnvironment::ProteinZone &a, const SimpleProteinEnvironment::ProteinZone&b , const BoundingBox3f3f &bbox);
  ///Relocate a zone to a new box location
  ProteinZone relocateZone(ProteinZone a, const BoundingBox3f3f &bbox);
  //Merge 2 zones of same gene type
  void combineZonesFromSameGene(ProteinZone &a, const ProteinZone&b);  
  ProteinZone& getZone(iterator it);
  const ProteinZone& getZone(iterator it)const;
  ///This function uses the currently selected protein zone to filter out the relevant gene for a given effect
  const Elysia::Genome::Gene& retrieveGeneHelper(ProteinZone* where, const Elysia::Genome::Effect&effect);
public:
  ///the first uniform protein zone in a reasonable ordering of zones
  iterator begin()const;
  ///just past the last uniform protein zone in a reasonable ordering of zones
  iterator end()const;
  ///increment an iterator (usually incrementing its count or offset) to point to the next zone so all protein zones are hit from the path of begin to end (not inclusive)
  void increment(iterator*)const;
  ///get the bounding box of a given unform region
  BoundingBox3f3f getBoundingBox(iterator )const;
  ///Initialize the main-zone-list by reading the genes and creating 1 zone per gene from Genome (and simplify)
  ProteinEnvironment& initialize(const Elysia::Genome::Genome&genes);
  /// returns an iterator to the location specified by the vector3f
  ProteinEnvironment::iterator getIterator(const Vector3f& where);
  ///Get protein density at a location (given location, and protein effect interested in)
  float getProteinDensity(const Vector3f &location, const Elysia::Genome::Effect&);
  ///Get protein density at a location (given location, and protein effect interested in)
  float getProteinDensity(iterator it, const Elysia::Genome::Effect&);
  ///Find all the proteins at a given point (given location)
  std::vector<EffectAndTypeAndDensity > getCompleteProteinDensity(const Vector3f& location);
  ///Find all the proteins at a given point (given location)
  std::vector<EffectAndTypeAndDensity > getCompleteProteinDensity(iterator it);
  ///Combine 2 zones by dividing it up into parts before reassembling them
  void chopZonePair(const ProteinZone &a, const ProteinZone &b, std::vector<ProteinZone> &output);
  ///Split and simplify large zone definitions into smaller component zones for calculations (given list of all zones)
  void zoneIntersection(std::vector<ProteinZone>& mMainZoneList);
  ///Rebuild the zone list such that it exclude the 2 zones currently in question
  void rebuildZones(std::vector<ProteinZone>::const_iterator a, std::vector<ProteinZone>::const_iterator b, const std::vector<ProteinZone> &input, std::vector<ProteinZone> &output);

  ///Zone management functions to add and remove zones from the main list
  void addZones(const std::vector<ProteinZone> &mSubZoneList,
               std::vector<ProteinZone> &mMainZoneList);
  void removeZones(std::vector<ProteinZone> mSubZoneList,
                  std::vector<ProteinZone> mMainZoneList);
    
  ///Update the zones to the next time step
  void updateAllEnvironmentSoup(float age);
  
  ///Function to find the zone that a single point reside in
  ProteinZone &resideInZones(const Vector3f queryPoint, 
                             const std::vector<ProteinZone> &mMainZoneList);
  ///This function looks through the genes present in the location and their densities and selects the gene most likely to have contributed to the desored effects' density at the moment.
  virtual const Elysia::Genome::Gene& retrieveGene(const Vector3f &location, const Elysia::Genome::Effect&effect);

  ///This function looks through the genes present in the location and their densities and selects the gene most likely to have contributed to the desored effects' density at the moment.
  virtual const Elysia::Genome::Gene& retrieveGene(iterator location, const Elysia::Genome::Effect&effect);

                             
  virtual BoundingBox3f3f getBounds()const;
  ~SimpleProteinEnvironment();
};
}

