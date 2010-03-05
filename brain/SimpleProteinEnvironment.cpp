#include "Platform.hpp"
#include "ProteinEnvironment.hpp"
#include "SimpleProteinEnvironment.hpp"
namespace Elysia {
/*
namespace std {
template <class T, class U> class pair {public:
 pair (const T&f, const U&s) {first=f;second=s;}
 T first;
 U second;
};}
*/

//Initialize the main-zone-list by reading the genes and creating 1 zone per gene from Genome
void SimpleProteinEnvironment::initialize(const Elysia::Genome::Genome&genes){
    //Zones start out as representing 1 gene (can be overlapping)
    //Since they can represent a collection of genes,
    //They are chopped during the intersection process and overlap regions become new zones
    const Elysia::Genome::Chromosome *fatherMother[2];
    fatherMother[0]=&genes.fathers();
    fatherMother[1]=&genes.mothers();
    //Loop through all the genes
    for (int chromosomeNum=0;chromosomeNum<2;++chromosomeNum) {
       int num_genes=fatherMother[chromosomeNum]->genes_size();
       for (int i=0;i<num_genes;++i) {
          //Define the new zone
          const Elysia::Genome::Gene*gene=&fatherMother[chromosomeNum]->genes(i);
          int num_bounds=gene->bounds_size();
          for (int k=0;k<num_bounds;++k) {
              ProteinZone newZone;
              newZone.mGenes.push_back(*gene);
              int num_proteins=gene->external_proteins_size();
              for (int j=0;j<num_proteins;++j) {
                  const Elysia::Genome::Protein *protein=&gene->external_proteins(j);
                  newZone.mSoup.push_back(ProteinZone::EffectAndDensityPair(protein->protein_code(),protein->density()));
              }
              newZone.mBounds =BoundingBox3f3f(Vector3f(gene->bounds(k).minx(),
                                                        gene->bounds(k).miny(),
                                                        gene->bounds(k).minz()),
                                               Vector3f(gene->bounds(k).maxx(),
                                                        gene->bounds(k).maxy(),
                                                        gene->bounds(k).maxz()));
              mMainZoneList.push_back(newZone);
          }
       }
    }
}

//Find the "specific protein density" total up the protein (given effect)
//Return the magnitude of a given effect. 
float SimpleProteinEnvironment::ProteinZone::getSpecificProteinDensity(Elysia::Genome::Effect e){
  float retval=0;
  std::vector< std::pair<Elysia::Genome::Effect,float> >::const_iterator i,ie;
  for (i=mSoup.begin(),ie=mSoup.end();i!=ie;++i) {
  //if the effect in the current iterator matches the desired protein effect passed in as "e", add the float to the return value
        if (i->first==e) retval+=i->second;
  }
  return retval;
}

//Get protein density at a location (given location, and protein effect interested in)
//Return float of the effect's density
float SimpleProteinEnvironment::getProteinDensity(const Vector3f &location, const Elysia::Genome::Effect&effect) {
  float density=0;
  ///loop through all protein zones
  for (std::vector<ProteinZone>::iterator i=mMainZoneList.begin(),ie=mMainZoneList.end();i!=ie;++i) {
    //if our test point is in the zone
    if (i->getBoundingBox().contains(location)) {
        //then accumulate the matching effect (if present) into final result
        density+=i->getSpecificProteinDensity(effect);
    }
  }
  //and return the result
  return density;
}

//Find all the proteins at a given point (given location)
//Return vector of proteins
std::vector<std::pair<Elysia::Genome::Effect, float> > SimpleProteinEnvironment::getCompleteProteinDensity(const Vector3f& location){
  std::vector<ProteinZone::EffectAndDensityPair >proteins;
  //loop through all the zones
  for (std::vector<ProteinZone>::iterator i=mMainZoneList.begin(),ie=mMainZoneList.end();i!=ie;++i) {
    //if our test point is in the zone
    if (i->getBoundingBox().contains(location)) {
        //Append the effects (i.e. mSoup) to the end of the returned proteins
        proteins.insert(proteins.end(),i->mSoup.begin(),i->mSoup.end());
    }
  }
  return proteins;
}

//Update ALL the mSoup (densities) for the next timestep, (given the current age)
//Return nothing
// These functions are not ready yet--- we should get LNK errors for them so we know when we're done
//"Complicated" function to update the soup for the next time iteration
void SimpleProteinEnvironment::ProteinZone::updateSoupToNextSoup(const float age){
	//Will write this function last
}


//Split large zone definitions into smaller component zones for calculations (given zone 1 and zone 2)
//Return a list of zones (post-split)
std::vector<SimpleProteinEnvironment::ProteinZone> SimpleProteinEnvironment::ZoneIntersection(const ProteinZone &zone1, 
                                                                    const ProteinZone &zone2){
	std::vector<ProteinZone> myZoneListReturn;
	return myZoneListReturn;
}

//Zone management functions to add zones to the main list (given sub-list, and main-list)
//Return new main-list
void SimpleProteinEnvironment::addZone( std::vector<ProteinZone> mSubZoneList, 
                                        std::vector<ProteinZone> mMainZoneList){
    //Append all the zones in the sub-list to the main-list
}

//Zone management functions to remove zones from the main list (given sub-list, and main-list)
//Return new main-list
void SimpleProteinEnvironment::removeZone(  std::vector<ProteinZone> mSubZoneList, 
                                            std::vector<ProteinZone> mMainZoneList){
    //Find and remove all matching entries in the sub-list out of the main-list
}

//Function to find the zone that a single point reside in (given point, and zone list)
//Return THE zone from the list that is inside the point, or fail
SimpleProteinEnvironment::ProteinZone &SimpleProteinEnvironment::resideInZones(   const Vector3f queryPoint, 
                                                        std::vector<ProteinZone> mMainZoneList){
	int i;
	static ProteinZone myFail;
	size_t numMainZones=mMainZoneList.size();

	for(size_t i=0;i<mMainZoneList.size();++i){
		if (mMainZoneList[i].mBounds.contains(queryPoint)) {
			return mMainZoneList[i];
		}
	}

	return myFail;
}

}
