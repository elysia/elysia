#include "Platform.hpp"
#include "SimpleProteinEnvironment.hpp"
namespace Elysia {
/*
//Example of a pair
namespace std {
template <class T, class U> class pair {public:
 pair (const T&f, const U&s) {first=f;second=s;}
 T first;
 U second;
};}
*/

//Initialize the main-zone-list by reading the genes and creating 1 zone per gene from Genome (and simplify)
ProteinEnvironment& SimpleProteinEnvironment::initialize(const Elysia::Genome::Genome&genes){
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
	zoneIntersection(mMainZoneList);
    return *this;
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

//The area is not equal in any aspect -> not OK
static bool okArea(const BoundingBox3f3f&input) { 
  return input.min().x!=input.max().x&&input.min().y!=input.max().y;
}

template <class T> BoundingBox<T> intersect(const BoundingBox<T>&a, const BoundingBox<T>&b) {
   BoundingBox<T> retval(a.min().max(b.min()),a.max().min(b.max()));
   if (retval.diag().x<=0||retval.diag().y<=0) {
      return BoundingBox<T>(retval.min(),retval.min());//null bounding box at min
   }
   return retval;
}

//Combine genes into a single-shared region
 SimpleProteinEnvironment::ProteinZone SimpleProteinEnvironment::combineZone(const SimpleProteinEnvironment::ProteinZone &a, const SimpleProteinEnvironment::ProteinZone&b , const BoundingBox3f3f &bbox) {
   ProteinZone retval;
   retval.mBounds=bbox;
   assert(!a.mSoup.empty()&&!b.mSoup.empty());
   //Combine the genes into a shared, common sized, region
   retval.mGenes.insert(retval.mGenes.end(),a.mGenes.begin(),a.mGenes.end());
   retval.mGenes.insert(retval.mGenes.end(),b.mGenes.begin(),b.mGenes.end());
   return retval;
}

//Relocate a zone to a new box location
SimpleProteinEnvironment::ProteinZone SimpleProteinEnvironment::relocateZone(ProteinZone a, const BoundingBox3f3f &bbox) { //generate a box of type a
   a.mBounds=bbox;
   return a;
}

//Merge 2 zones of same gene type
void SimpleProteinEnvironment::combineZonesFromSameGene(ProteinZone &a, const ProteinZone&b) {  
   a.mBounds.mergeIn(b.mBounds);
}

//Combine 2 zones by dividing it up into parts before reassembling them
void SimpleProteinEnvironment::chopZonePair(const ProteinZone &a, const ProteinZone &b, std::vector<ProteinZone> &output) {
    // Assume any combination of 2 zones will results in 9 subsections
    // Figure out using the intersections points how to generate the subsections
    // Subsections of area 0 --> ignore
    // Add subsections of significant areas to the list with the proper gene type
    // Combine subsections of similar gene type horizontally
  ProteinZone ab[2]={a,b};  //combine a and b into a list
  if (!okArea(intersect(ab[0].mBounds,ab[1].mBounds))) {
     output.push_back(a);
     output.push_back(b);
     return;
  }
  float horizedges[4]={ab[0].mBounds.min().x,ab[1].mBounds.min().x,ab[0].mBounds.max().x,ab[1].mBounds.max().x};  //generate list of x bounds
  float vertedges[4]={ab[0].mBounds.min().y,ab[1].mBounds.min().y,ab[0].mBounds.max().y,ab[1].mBounds.max().y};  //generate lsit of y bounds
  std::sort(horizedges+0,horizedges+4);  //Sort the horizontal list left->right (min->max)
  std::sort(vertedges+0,vertedges+4);  //Sort the vertical list top->bottom (min->max)
  bool combinable[3][3][2];
  ProteinZone combiners[3][3][2];
  //Loop through the subsections
  for (int i=0;i<3;++i) {
    for (int j=0;j<3;++j) {
      BoundingBox3f3f newBounds[2];
      //Generate one section 1->9 (corner-to-corner)
      BoundingBox3f3f location(Vector3f(horizedges[i],vertedges[j],a.mBounds.min().z),
                                   Vector3f(horizedges[i+1],vertedges[j+1],a.mBounds.max().z));
      bool valid[2];
      for (int z=0;z<2;++z) {
         //Is it inside a or b?
         valid[z]=okArea(newBounds[z]=intersect(location,ab[z].mBounds));
         combinable[i][j][z]=false;
      }
      if (valid[0]&&valid[1]) { //It is a subsection of A and B
         output.push_back(combineZone(a,b,location));
      } else if (valid[0]) { //It is a subsection of A
         combiners[i][j][0]=relocateZone(ab[0],newBounds[0]);
         combinable[i][j][0]=true;
      } else if (valid[1]) { //It is a subsection of B
         combiners[i][j][1]=relocateZone(ab[1],newBounds[1]);
         combinable[i][j][1]=true;
      }
    }
  }
  //Results in 9 subsections of type A, B and AB
  
  //Loop through the subsections and determine zones that are neighbors (horizontal)
  for (int z=0;z<2;++z) {
    for (int i=0;i<3;++i) {
      for (int j=0;j<2;++j) {
         if (combinable[i][j][z]&&combinable[i][j+1][z]) {
            combineZonesFromSameGene(combiners[i][j+1][z],combiners[i][j][z]);
            combinable[i][j][z]=false;
         }
       }
    }
  }
  //send back combined zones
  for (int z=0;z<2;++z) {
    for (int i=0;i<3;++i) {
      for (int j=0;j<3;++j) {
         if (combinable[i][j][z]) {
            output.push_back(combiners[i][j][z]);
         }
      }
    }
  }
}

//Split and simplify large zone definitions into smaller component zones for calculations (given list of all zones)
//Return a list of zones (post-split)
void SimpleProteinEnvironment::zoneIntersection(std::vector<ProteinZone> mMainZoneList){
	std::vector<ProteinZone> mLocalZoneList;
	std::vector<ProteinZone> mSwapZoneList;
	mLocalZoneList = mMainZoneList;
	mSwapZoneList = mMainZoneList;
	bool converged=false;
	bool restart=false;

	while (!converged) {
		//Defaults to converged incase for-loop and detection never terminates
    	converged=true;

    	//Loop through zones and check intersection (for row from start-->end)
    	for (std::vector<ProteinZone>::iterator i=mMainZoneList.begin(),ie=mMainZoneList.end();i!=ie;++i) {
			//(for column from current-row-diagonal + 1 --> end) ==> results in half triangle w/o diagonal
	    	for (std::vector<ProteinZone>::iterator j=i+1,je=mMainZoneList.end();j!=je;++j) {
				if(!restart){
					mLocalZoneList.clear();	//ensure that the list returned is initialized properly
					mSwapZoneList.clear();		//ensure that the list returned is initialized properly
					chopZonePair(*i,*j,mLocalZoneList);
					if(mLocalZoneList.size() == 2){
						if( ((i->mBounds == mLocalZoneList[0].mBounds)  || 
							 (i->mBounds == mLocalZoneList[1].mBounds)) ||
							((j->mBounds == mLocalZoneList[0].mBounds)  || 
							 (j->mBounds == mLocalZoneList[1].mBounds)) ){
							//Technicaly, I only need to look at only i, or j, but the 2nd set it a sanity check
							//No new zones were created
							//-No action
						}else{
							//These are 2 new zones
							//-Create new zones excluding current 2
							//-Append new zones to list
							//-Restart -> Swap
							rebuildZones(i,j,mMainZoneList,mSwapZoneList);
							addZones(mLocalZoneList,mSwapZoneList);
							restart = true;
						}
					}else if ((mLocalZoneList.size() > 2)||(mLocalZoneList.size() == 1)){
						//These are all new zones
						//-Create new zones excluding current 2
						//-Append new zones to list
						//-Restart -> Swap
						rebuildZones(i,j,mMainZoneList,mSwapZoneList);
						addZones(mLocalZoneList,mSwapZoneList);
						restart = true;
					}else{
						//Serious error... should not be 0 or fewer
					}
				}//end if
    		}//end for
    	}//end for

		if(restart){
			mMainZoneList.swap(mSwapZoneList);
			converged=false;
			restart=false;
		}

	}//end while
}

//Rebuild the zone list such that it exclude the 2 zones currently in question
//This allows us to throw out the 2 zones and add their replacements
void SimpleProteinEnvironment::rebuildZones(std::vector<ProteinZone>::const_iterator a,std::vector<ProteinZone>::const_iterator b, const std::vector<ProteinZone> &input, std::vector<ProteinZone> &output) {
    // Create a new list excluding the current 2 zones
	for (std::vector<ProteinZone>::const_iterator i=input.begin(),ie=input.end();i!=ie;++i) {
		if( (i != a)&&(i != b) ){
			output.push_back(*i);
		}
	}
	return;
}

//Zone management functions to add zones to the main list (given sub-list, and main-list)
//Return new main-list
void SimpleProteinEnvironment::addZones( const std::vector<ProteinZone> &mSubZoneList, 
                                        std::vector<ProteinZone> &mMainZoneList){
    //Append all the zones in the sub-list to the main-list
	for (std::vector<ProteinZone>::const_iterator i=mSubZoneList.begin(),ie=mSubZoneList.end();i!=ie;++i) {
		mMainZoneList.push_back(*i);
	}
	return;
}

//Zone management functions to remove zones from the main list (given sub-list, and main-list)
//Return new main-list
void SimpleProteinEnvironment::removeZones(  std::vector<ProteinZone> mSubZoneList, 
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

const Elysia::Genome::Gene& SimpleProteinEnvironment::retrieveGene(const Vector3f &location, const Elysia::Genome::Effect&effect){
  assert(0);
  //FIXME actually look up the responsible gene from the set of "active" genes causing the effect to be spilled at this location
  static Elysia::Genome::Gene retval;
  return retval;
}

SimpleProteinEnvironment::~SimpleProteinEnvironment(){
}

}
