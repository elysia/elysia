#include "Platform.hpp"
#include "ProteinEnvironment.hpp"
#include "SimpleProteinEnvironment.hpp"
namespace Elysia {

float SimpleProteinEnvironment::ProteinZone::getSpecificProteinDensity(Elysia::Genome::Effect e){
   float retval=0;
   std::vector< std::pair<Elysia::Genome::Effect,float> >::const_iterator i,ie;
   for (i=mSoup.begin(),ie=mSoup.end();i!=ie;++i) {
        if (i->first==e) retval+=i->second;
  }
  return retval;
}


float SimpleProteinEnvironment::getProteinDensity(const Vector3f &location, const Elysia::Genome::Effect&effect) {
  float density=0;
   ///loop through all protein zones
  for (std::vector<ProteinZone>::iterator i=mMainZoneList.begin(),ie=mMainZoneList.end();i!=ie;++i) {
    //if the effect of the current protein matches what we're looking for and the current zone contains the given location 
    if (i->getBoundingBox().contains(location)) {
        //then we accumulate
        density+=i->getSpecificProteinDensity(effect);
    }
  }
   //and return the result
  return density;
}

std::vector<std::pair<Elysia::Genome::Effect, float> > SimpleProteinEnvironment::getCompleteProteinDensity(const Vector3f& location){
  std::vector<ProteinZone::EffectAndDensityPair >proteins;
  for (std::vector<ProteinZone>::iterator i=mMainZoneList.begin(),ie=mMainZoneList.end();i!=ie;++i) {
    if (i->getBoundingBox().contains(location)) {
        proteins.insert(proteins.end(),i->mSoup.begin(),i->mSoup.end());
    }
  }
  return proteins;
}
// These functions are not ready yet--- we should get LNK errors for them so we know when we're done
//"Complicated" function to update the soup for the next time iteration
void SimpleProteinEnvironment::ProteinZone::updateSoupToNextSoup(const float age){
	//Will write this function last
}

//Split large zone definitions into smaller component zones for calculations
std::vector<SimpleProteinEnvironment::ProteinZone> SimpleProteinEnvironment::ZoneIntersection(const ProteinZone &zone1, 
                                                                    const ProteinZone &zone2){
	std::vector<ProteinZone> myZoneListReturn;
	return myZoneListReturn;
}

//Zone management functions to add and remove zones from the main list
void SimpleProteinEnvironment::addZone( std::vector<ProteinZone> mSubZoneList, 
                                        std::vector<ProteinZone> mMainZoneList){
}

void SimpleProteinEnvironment::removeZone(  std::vector<ProteinZone> mSubZoneList, 
                                            std::vector<ProteinZone> mMainZoneList){
}

//Function to find the zone that a single point reside in
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