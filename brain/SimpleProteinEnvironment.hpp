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

public:
  ProteinEnvironment& initialize(const Elysia::Genome::Genome&genes);
  float getProteinDensity(const Vector3f &location, const Elysia::Genome::Effect&);
  std::vector<std::pair<Elysia::Genome::Effect, float> > getCompleteProteinDensity(const Vector3f& location);

  //Split large zone definitions into smaller component zones for calculations
  std::vector<ProteinZone> ZoneIntersection(const ProteinZone &zone1, 
                                            const ProteinZone &zone2);
  //Zone management functions to add and remove zones from the main list
  void addZone(std::vector<ProteinZone> mSubZoneList,
               std::vector<ProteinZone> mMainZoneList);
  void removeZone(std::vector<ProteinZone> mSubZoneList,
                  std::vector<ProteinZone> mMainZoneList);

  //Function to find the zone that a single point reside in
  ProteinZone &resideInZones(const Vector3f queryPoint, 
                             std::vector<ProteinZone> mMainZoneList);
  ~SimpleProteinEnvironment();
};
}

/**TEMPORARY NOTES TO SELF (NIA) --- From genes to zone entities

*/

/**TEMPORARY NOTES TO SELF (NIA) --- Simple splitting algorithm
    Need:
        find box
            list{box} = boxIntersection(boxA, boxB) [spatial, 2D version] 
        index/track box
            addBoxToList(box) //myVector.insert(myVector.end(),otherVector.begin(),otherVector.end());
            removeBoxFromList(box)
        box operations
            list{boxIDs} = resideInBoxes(point)

    Externals Functions I need:
        Spatial Library
            isInBox(point,box)

    Sample Problem:
    (Algorithm 1)
        Initialization:
            Gene read --> create box A, box B, box C
            Add box A to list
            Add box B to list
            Add box C to list
            Start check...
                Found: box A overlaps with box B
                    --> Remove box A
                    --> Remove box B
                    --> Create box D = AB
                    --> Create box E = A1
                    --> Create box F = B1
                    --> Create box G = B2
                    --> Create box H = B3
                    --> Restart
            Start check...
                Found: box B3 overlaps with box C
                    --> Split (but part of C may still overlap with A1)
                    --> Restart
            Start check...
                Eventually... there will be no overlaps

    Function to update protein:
        @t=0
            Soup=A
            Genes=abcd --> do some processing to figure out what soup comes next
            --> update soup to B
        @t=1
            Soup=B
            Genes=abcd --> do some processing to figure out what soup comes next
            --> update soup to C
*/
