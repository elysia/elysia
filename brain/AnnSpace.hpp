#include <vector>
#include "AnnPoints.hpp"
#include "Neuron.hpp"
#include "STANNSpatialSearch.hpp"


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
		void partitionSpace();
		void deletePoint(Neuron* neuron, STANNSpatialSearch* stann);
		void addPoint(Neuron* neuron, STANNSpatialSearch* stann);
		Neuron* findNN(float x, float y, Neuron* exclude);
		void mergeSpace();
	};
}
