#include <vector>
#include "AnnPoints.hpp"
#include "TreeNNSpatialSearch.hpp"
#include "Placeable.hpp"
#include <cfloat>

namespace Elysia {
	class BRAIN_CORE_EXPORT AnnSpace{
		enum WhichAxis {XAXIS, YAXIS};
		WhichAxis whichAxis;
		float partitionPoint;
		AnnSpace* child[2];
		AnnSpace* parent;
		std::vector<Placeable*> placeableList;
	public:
		AnnSpace();
		~AnnSpace();
		int chooseChild(float x, float y);
		bool isLeaf();

		void partitionSpace(TreeNNSpatialSearch* treenn);
		void deletePoint(Placeable* Placeable, TreeNNSpatialSearch* treenn);
		void addPoint(Placeable* Placeable, TreeNNSpatialSearch* treenn);
		void setPartition(float newPartition){partitionPoint = newPartition;}
		void setParent(AnnSpace* newParent){parent = newParent;}
		Placeable* findNN(float x, float y, Placeable* exclude);
		void mergeSpace(TreeNNSpatialSearch* treenn);
		std::vector<Placeable*> getChildList();
		AnnSpace* findSibling(AnnSpace* offspring);
		void resetChild(AnnSpace* departingChild, AnnSpace* newChild);
	};
}
