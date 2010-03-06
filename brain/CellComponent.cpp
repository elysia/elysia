#include "Platform.hpp"
#include "CellComponent.hpp"
#include "Branch.hpp"
#include "ActivityStats.hpp"
namespace Elysia {
CellComponent::CellComponent() {
    //These are default values for a generic neuron
    mActivity = 0;
    mThreshold = 1;
    mDevelopmentStage = 0;
    mSignalStrength = 1;
}

void CellComponent::syncBranchDensity(float parentRandomBranchDeterminer, float parentRandomDepthDeterminer, float baseBranchiness, float tipBranchiness, float treeDepth, int depth) {
    float branchiness;
    if(depth>parentRandomDepthDeterminer/2){
        branchiness = tipBranchiness;
    }
    else{branchiness = baseBranchiness;}

    assert(branchiness<(1<<23) );//using floats in a manner that will not cooperate with floats after a few millions
    size_t childSize=mChildBranches.size();
    float roundingError=branchiness-floor(branchiness);
    if (parentRandomBranchDeterminer>roundingError) {
       branchiness=ceil(branchiness);
    }else {
       branchiness=floor(branchiness);
    }
    size_t maxDepth=floor(parentRandomDepthDeterminer);
    if (parentRandomDepthDeterminer>treeDepth-floor(treeDepth))
      maxDepth=ceil(treeDepth);
    if (depth>=(int)maxDepth) {
        branchiness=0;
    }    
    branchiness-=childSize;
    while(branchiness>0){
       Branch *b;
       mChildBranches.push_back(b = new Branch(this));
       if(depth>=(int)maxDepth-1){
            b->growSynapse();
       }
       branchiness-=1;
    }
    while (branchiness<0&&mChildBranches.size()) {
       size_t whichDies=rand()%mChildBranches.size();
       delete mChildBranches[whichDies];
       if (whichDies+1!=mChildBranches.size()) 
           mChildBranches[whichDies]=mChildBranches.back();
       mChildBranches.pop_back();
    }    
    for (size_t i=0;i<mChildBranches.size();++i) {
        mChildBranches[i]->syncBranchDensity(parentRandomBranchDeterminer, parentRandomDepthDeterminer, baseBranchiness, tipBranchiness, treeDepth, depth + 1);
    }
}

}
