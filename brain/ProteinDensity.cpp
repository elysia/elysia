#include "Platform.hpp"
#include "genome.pb.h"
#include "ProteinDensity.hpp"
namespace Elysia {

/**
 *	@param const BoundingBox3f3f &bb - a bound on the random distribution
 *	@returns a 3-float vector with a random position in some box
**/
Vector3f randomWithinBbox(const BoundingBox3f3f&bb) {
    return Vector3f(bb.min().x+
                    (bb.max().x-bb.min().x)*(double)rand()/RAND_MAX,
                    bb.min().y+
                    (bb.max().y-bb.min().y)*(double)rand()/RAND_MAX,
                    bb.min().z+
                    (bb.max().z-bb.min().z)*(double)rand()/RAND_MAX);
}

class PairFirstLess{
public:
    template <typename T> bool operator()(const T &a, const T&b)const {
        return a.first<b.first;
    }
};

/**
 *	@param const Elysia::Genome::TemporalBoundingBox &tbb - source bounding box
 *	@returns a 3-float vector converted from a TemporalBoundingBox type
**/
BoundingBox3f3f convertBB(const Elysia::Genome::TemporalBoundingBox&tbb) {
    return BoundingBox3f3f(Vector3f(tbb.minx(),
                                    tbb.miny(),
                                    tbb.has_maxz()?tbb.maxz():0),
                           Vector3f(tbb.maxx(),
                                    tbb.maxy(),
                                    tbb.has_maxz()?tbb.maxz():0));
}

/**
 *	@param float age - age of some bounding box
 *	@param const Elysia::Genome::TemporalBoundingBox &tbb - bounding box to check
 *	@returns TRUE or FALSE (see description)
 *
 *	Description:	Returns TRUE if one of: tbb doesn't have a minimum t | minimum t is less than age
 *																AND one of
 *											tbb has no maximum t | maximum t is more than age
 *					Otherwise FALSE
**/
bool isActiveBB(float age, const Elysia::Genome::TemporalBoundingBox&tbb) {
    if ((tbb.has_mint()==false||
         tbb.mint()<=age)&&
        (tbb.has_maxt()==false||
         tbb.maxt()>=age)) {
        return true;
    }
    return false;
}

/**
 *	@param float age - age of some bounding box
 *	@param std::vector<BoundingBox3f3f> &eligibleBoxes - vector of eligible bounding boxes
 *
 *	Description:	Looks through a vector of target regions from a gene.  If if the target region is
 *					an active bounding box (isActiveBB), then it is added to a vector of eligible
 *					bounding boxes
**/
void ProteinDensity::getTargetBounds(float age, std::vector<BoundingBox3f3f> &eligibleBoxes){
    int num_regions=this->mGene.target_region_size();
    for(int i=0;i<num_regions;++i) {
        const Elysia::Genome::TemporalBoundingBox *tbb = &mGene.target_region(i);
        if (isActiveBB(age,*tbb)) {
            eligibleBoxes.push_back( convertBB(*tbb));
        }
    }    
}

/**
 *	@param float age - age of some bounding box
 *	@returns a 3d vector of random points
 *
 *	Description:	Finds the number of regions in a gene, then tries twice to return a random point within a box.
 *					If this doesn't succeed, then the function calls getTargetBounds() from above.  If there's at least 
 *					one item in eligibleBoxes, then a random coordinate is returned from there, otherwise return 0,0,0
**/
Vector3f ProteinDensity::getRandomTargetPointByArea(float age) {
    int num_regions=this->mGene.target_region_size();
    if (num_regions) {
        int num_tries=2;
        for (int i=0;i<num_tries;++i) {
            const Elysia::Genome::TemporalBoundingBox*tbb=&mGene.target_region(rand()%num_regions);
            if (isActiveBB(age,*tbb)) {
                return randomWithinBbox(convertBB(*tbb));
            }
        }
    }
    std::vector<BoundingBox3f3f> eligibleBoxes;   
    getTargetBounds(age,eligibleBoxes);
    if (eligibleBoxes.size()) {
        return randomWithinBbox(eligibleBoxes[rand()%eligibleBoxes.size()]);
    }
    return Vector3f(0,0,0);
    
}

/**
 *	@param float age - age of some bounding box
 *	@returns a 3d vector of random points
 *
 *	Description:	
**/
Vector3f ProteinDensity::getRandomTargetPointByRegion(float age) {
    std::vector<std::pair<float,BoundingBox3f3f> >eligibleBoxes;
    float totalArea=0;
    int num_regions=this->mGene.target_region_size();
    for(int i=0;i<num_regions;++i) {
        const Elysia::Genome::TemporalBoundingBox*tbb=&mGene.target_region(i);
        
        if (isActiveBB(age,*tbb)) {
            float area=(tbb->maxx()-tbb->minx())*
                (tbb->maxy()-tbb->miny());
            float newTotal=totalArea+area;
            if (newTotal!=totalArea) {
                eligibleBoxes.push_back(std::pair<float,BoundingBox3f3f>(totalArea,
                                                                          convertBB(*tbb)));
                totalArea=newTotal;
            }
        }
    }
    std::vector<std::pair<float,BoundingBox3f3f> >::iterator where=
        std::lower_bound(eligibleBoxes.begin(),
                     eligibleBoxes.end(),
                         std::pair<float,BoundingBox3f3f>((rand()/(double)RAND_MAX)*totalArea,BoundingBox3f3f()),
                     PairFirstLess());
    if (where!=eligibleBoxes.begin()) {
        --where;
    }
    if (where!=eligibleBoxes.end()) {
        return randomWithinBbox(where->second);
    }
    return Vector3f(0,0,0);
}
}
