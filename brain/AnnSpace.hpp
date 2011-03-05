#include <vector>
#include "AnnPoints.hpp"
#include "Neuron.hpp"
#include "TreeNNSpatialSearch.hpp"


namespace Elysia {
	class AnnSpace{
		enum WhichAxis {XAXIS, YAXIS};
		WhichAxis whichAxis;
		float partitionPoint;
		AnnSpace* child[2];
		AnnSpace* parent;
		std::vector<Neuron*> neuronList;
	public:
		AnnSpace();
		int chooseChild(float x, float y);
		bool isLeaf();
		void partitionSpace(TreeNNSpatialSearch* stann);
		void deletePoint(Neuron* neuron, TreeNNSpatialSearch* stann);
		void addPoint(Neuron* neuron, TreeNNSpatialSearch* stann);
		void setParent(AnnSpace* newParent){parent = newParent;}
		Neuron* findNN(float x, float y, Neuron* exclude);
		void mergeSpace();
	};
}
