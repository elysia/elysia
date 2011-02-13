#include <vector>
#include "AnnPoints.hpp"

namespace Elysia {
	class AnnSpace{
		int mPointPartitionThreshold;
		int mNumPoints;
		std::vector<AnnPoints*> mPointList;

		float mBoundingBox[2][2];			//1st array dimension indicates the spatial dimension, the 2nd array dimension represents 0 min, 1 max in that dimension
	
		//functions
		AnnSpace* partitionSpace();
		void setBoundingBox(float box[2][2]);

	};
}
