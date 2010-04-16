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

Vector3f ProteinDensity::getRandomTargetPoint(float age) {
    std::vector<std::pair<float,BoundingBox3f3f> >mEligibleBoxes;
    float totalArea=0;
    int num_regions=this->mGene.target_region_size();
    for(int i=0;i<num_regions;++i) {
        const Elysia::Genome::TemporalBoundingBox*tbb=&mGene.target_region(i);
        
        if ((tbb->has_mint()==false||
             tbb->mint()<=age)&&
            (tbb->has_maxt()==false||
             tbb->maxt()>=age)) {
            float area=(tbb->maxx()-tbb->minx())*
                (tbb->maxy()-tbb->miny());
            float newTotal=totalArea+area;
            if (newTotal!=totalArea) {
                mEligibleBoxes.push_back
                    (std::pair<float,BoundingBox3f3f>(totalArea,
                                                      BoundingBox3f3f(Vector3f(tbb->minx(),
                                                                               tbb->miny(),
                                                                               tbb->has_maxz()?tbb->maxz():0),
                                                                      Vector3f(tbb->maxx(),
                                                                               tbb->maxy(),
                                                                               tbb->has_maxz()?tbb->maxz():0))));
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
