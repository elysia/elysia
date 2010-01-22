
class ProteinDeposit;

/** Define the protein soup that components interact with
 */
class SimpleProteinEnvironment{
  std::vector<ProteinDeposit> mProteins;//these two things

  //Define the list of bounding regions and respective identifiers
  struct SingleBox{
    std::vector<Elysia::Genome::Gene> mGenes;
    BoundingBox3f3f mBounds;
    std::vector<std::pair<Elysia::Genome::Effect,float> > mSoup;  
  };
  std::vector<SingleBox> mMainBoxList;
  std::vector<SingleBox> mSubBoxList;
private:
public:
  float getProteinDensity(const Vector3 &location, const Elysia::Genome::ProteinEffect&);
  std::vector<std::pair<Elysia::Genome::ProteinEffect, float> > getCompleteProteinDensity(const Vector3& location);

  //Define bounding box interaction functions
  //find box geometry and dice it up at initilization
  std::vector<SingleBox> boxIntersection(const SingleBox& box1, 
                                         const SingleBox& box2);
  //add boxes to main list
  void addBoxes(std::vector<SingleBox> mSubBoxList,
                std::vector<SingleBox> mMainBoxList);
  void removeBox(std::vector<SingleBox> mSubBoxList,
                 std::vector<SingleBox> mMainBoxList);
};

/**TEMPORARY NOTES TO SELF (NIA)
    Need:
        find box
            list{box} = boxIntersection(boxA, boxB) [spatial, 2D version] 
        index/track box
            addBoxToList(box) //myVector.insert(myVector.end(),otherVector.begin(),otherVector.end());
            removeBoxFromList(box)
        box operations
            list{boxIDs} = resideInBoxes(point)
            readBoxInfo(boxID)

    Externals Functions I need:
        Spatial Library
            isInBox(point,box)

    Later functions:
        Temporal Tracker
            list{boxIDs} = findActiveBoxes(timestamp)

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
*/