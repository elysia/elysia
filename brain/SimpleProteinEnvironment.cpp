#include "Platform.hpp"
#include "SimpleProteinEnvironment.hpp"
namespace Elysia {
float SimpleProteinEnvironment::getProteinDensity(const Vector3 &location, const Elysia::Genome::ProteinEffect&effect) {
  float density=0;
  for (std::vector<ProteinDeposit>::iterator i=mProteins.begin(),ie=mProteins.end();i!=ie;++i) {
    if (i->getEffect()==effect&&i->getBoundingBox().contanis(location)) {
        density+=i->currentDensity;
    }
  }
  return density;
}

std::vector<std::pair<Elysia::Genome::ProteinEffect, float> > SimpleProteinEnvironment::getCompleteProteinDensity(const Vector3& location){
  std::vector<std::pair<Elysia::Genome::ProteinEffect, float> >proteins;
  for (std::vector<ProteinDeposit>::iterator i=mProteins.begin(),ie=mProteins.end();i!=ie;++i) {
    if (i->getBoundingBox().contanis(location)) {
        proteins.push_back(std::pair<Elysia::Genome::ProteinEffect,float>(i->getEffect(),density);
    }
  }
  return proteins;
}

//"Complicated" function to update the soup for the next time iteration
void SimpleProteinEnvironment::ProteinZone::updateSoupToNextSoup(const float age){
}

//Split large zone definitions into smaller component zones for calculations
std::vector<ProteinZone> SimpleProteinEnvironment::ZoneIntersection(const ProteinZone &zone1, 
                                                                    const ProteinZone &zone2){
}

//Zone management functions to add and remove zones from the main list
void SimpleProteinEnvironment::addZone( std::vector<ProteinZone> mSubZoneList, 
                                        std::vector<ProteinZone> mMainZoneList){
}

void SimpleProteinEnvironment::removeZone(  std::vector<ProteinZone> mSubZoneList, 
                                            std::vector<ProteinZone> mMainZoneList){
}

//Function to find the zone that a single point reside in
ProteinZone &SimpleProteinEnvironment::resideInZones(   const Vector3f queryPoint, 
                                                        std::vector<ProteinZone> mMainZoneList){
}
}