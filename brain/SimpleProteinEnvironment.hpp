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
  void ChopZonePair(const ProteinZone &a, const ProteinZone &b, std::vector<ProteinZone> &output);

  //Split large zone definitions into smaller component zones for calculations
  void ZoneIntersection(std::vector<ProteinZone> mMainZoneList);
  //Zone management functions to add and remove zones from the main list
  void addZone(std::vector<ProteinZone> mSubZoneList,
               std::vector<ProteinZone> mMainZoneList);
  void removeZone(std::vector<ProteinZone> mSubZoneList,
                  std::vector<ProteinZone> mMainZoneList);

  //Function to find the zone that a single point reside in
  ProteinZone &resideInZones(const Vector3f queryPoint, 
                             std::vector<ProteinZone> mMainZoneList);
  ///This function looks through the genes present in the location and their densities and selects the gene most likely to have contributed to the desored effects' density at the moment.
  virtual const Elysia::Genome::Gene& retrieveGene(const Vector3f &location, const Elysia::Genome::Effect&effect);

                             
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

//option 1
				//Compare the two zones
				//	1. Phase 1: Classify relationship
				//		A. full overlap
				//		B. crossing corners --> generate 3
				//		C. crossing sides --> generate 1
				//      D. Totally inside
				//		D. no crossing --> do nothing
				//  2. Phase 2: Generate new temporary list for overlap (perfect identical overlap)
				//     Goal: Generate 1 new zone from N overlapping zones --> combine chemicals
				//			>>Add itself to "merge" list
				//			>>Add all matching entries to "merge" list
				//			>>Run merge operation
				//			>>If only itself --> it doesn't overlap --> "merge" will yield just itself
				//			>>Add merge result into "new" list and swap with old one
				//			>>Loop through all until there are no overlaps remaining
				//  3. Phase 3: Detect partial overlaps
				//     Goal: Break up unique areas
				//          >>Use table driven cutting
				//          >>Add new zones to "new" list
				//          >>Swap with old list
				//          >>Restart detection from start --> including merge operations
				//          >>End if no new overlaps exist anywhere
//option 2
// for each edge in a, intersect it with perpendicular lines in zone b.  this gives between 0 and 2 intersections per edge
// Given edges A1,A2,A3,A4 & Given edges B1,B2,B3,B4
// --> find intersection points between A and B per edge
// --> Example: Edge A1 (top clockwise) --> may intersect with edges B1 (full or partial), B2 (point), B4 (point), or NONE
// Assume any combination of 2 zones will results in 9 subsections
// Figure out using the intersections points how to generate the subsections
// Subsections of area 0 --> ignore
// Add subsections of significant areas to the list with the proper gene type
// Combine subsections of similar gene type horizontally
            
*/
