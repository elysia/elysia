#include "Platform.hpp"
#include "genome.pb.h"
#include "ProteinDensity.hpp"
namespace Elysia {

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

BoundingBox3f3f convertBB(const Elysia::Genome::TemporalBoundingBox&tbb) {
    return BoundingBox3f3f(Vector3f(tbb.minx(),
                                    tbb.miny(),
                                    tbb.has_maxz()?tbb.maxz():0),
                           Vector3f(tbb.maxx(),
                                    tbb.maxy(),
                                    tbb.has_maxz()?tbb.maxz():0));
}
bool isActiveBB(float age, const Elysia::Genome::TemporalBoundingBox&tbb) {
    if ((tbb.has_mint()==false||
         tbb.mint()<=age)&&
        (tbb.has_maxt()==false||
         tbb.maxt()>=age)) {
        return true;
    }
    return false;
}
void ProteinDensity::getTargetBounds(float age, std::vector<BoundingBox3f3f>&eligibleBoxes){
    int num_regions=this->mGene.target_region_size();
    for(int i=0;i<num_regions;++i) {
        const Elysia::Genome::TemporalBoundingBox*tbb=&mGene.target_region(i);
        if (isActiveBB(age,*tbb)) {
            eligibleBoxes.push_back( convertBB(*tbb));
        }
    }    
}

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
Vector3f ProteinDensity::getRandomTargetPointByRegion(float age) {
    std::vector<std::pair<float,BoundingBox3f3f> >mEligibleBoxes;
    float totalArea=0;
    int num_regions=this->mGene.target_region_size();
    for(int i=0;i<num_regions;++i) {
        const Elysia::Genome::TemporalBoundingBox*tbb=&mGene.target_region(i);
        
        if (isActiveBB(age,*tbb)) {
            float area=(tbb->maxx()-tbb->minx())*
                (tbb->maxy()-tbb->miny());
            float newTotal=totalArea+area;
            if (newTotal!=totalArea) {
                mEligibleBoxes.push_back(std::pair<float,BoundingBox3f3f>(totalArea,
                                                                          convertBB(*tbb)));
                totalArea=newTotal;
            }
        }
    }
    std::vector<std::pair<float,BoundingBox3f3f> >::iterator where=
        std::lower_bound(mEligibleBoxes.begin(),
                     mEligibleBoxes.end(),
                         std::pair<float,BoundingBox3f3f>((rand()/(double)RAND_MAX)*totalArea,BoundingBox3f3f()),
                     PairFirstLess());
    if (where!=mEligibleBoxes.begin()) {
        --where;
    }
    if (where!=mEligibleBoxes.end()) {
        return randomWithinBbox(where->second);
    }
    return Vector3f(0,0,0);
}
}
