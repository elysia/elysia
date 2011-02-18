#include "AnnSpace.hpp"


namespace Elysia{


	AnnSpace* AnnSpace::partitionSpace(){

		return this;
	}
	void AnnSpace::setBoundingBox(float box[2][2]){
        for (int i=0;i<2;++i)
            for (int j=0;j<2;++j)
                mBoundingBox[i][j]=box[i][j];
	}
	
	//}
}
