#include "AnnSpace.hpp"


namespace Elysia{


	AnnSpace::AnnSpace(){
		child[0] = NULL;
		child[1] = NULL;
		parent = NULL;
	}

	AnnSpace::~AnnSpace(){
		if(child[0] != NULL){
			delete child[0];
		}
		if(child[1] != NULL){
			delete child[1];
		}
	}

	bool AnnSpace::isLeaf(){
		if (child[0] == NULL && child[1] == NULL){
			return true;
		}
		else{
			return false;
		}
	}

	int AnnSpace::chooseChild(float x, float y){
		if(whichAxis == XAXIS){
			if(x > partitionPoint){
				return 1;
			}
			else{
				return 0;
			}
		}
		else{
			if(y  > partitionPoint){return 1;}
			else{return 0;}
		}
	}

	class ComparePlaceablebyY {
		public:
		bool operator() (const Placeable*a, const Placeable*b) const {
		return a->getLocation().y<b->getLocation().y;
		}
	};
	class ComparePlaceablebyX {
		public:
		bool operator() (const Placeable*a, const Placeable*b) const {
		return a->getLocation().x<b->getLocation().x;
		}
	};

	void AnnSpace::partitionSpace(TreeNNSpatialSearch* treenn){
		std::vector<float> XArray;
		std::vector<float> YArray;
		for(std::vector<Placeable*>::iterator i=placeableList.begin();i!=placeableList.end();++i){
			Placeable* current=*i;
			Vector3f location  = current->getLocation();
			XArray.push_back(location.x);
			YArray.push_back(location.y);
		}
		child[0] = new AnnSpace();
		child[1] = new AnnSpace();
		child[0]->setParent(this);
		child[1]->setParent(this);
		//This algorithm is a super primitive way of partitioning the space. Something like k-means might be better
		//As the number of points becomes large, I expect the exactness of this for ANN improves and it's not that
		//bad to begin with. If someone wants to put k-means in here (or some other thing for calling partitions), cool.
		sort(XArray.begin(), XArray.end());
		sort(YArray.begin(), YArray.end());
		float Xdist = XArray[int(XArray.size()/2) + 1] - XArray[int(XArray.size()/2)];
		float Ydist = YArray[int(YArray.size()/2) + 1] - YArray[int(YArray.size()/2)];
		
		if(Xdist > Ydist){
			
			whichAxis = XAXIS;
			std::sort(placeableList.begin(),placeableList.end(),ComparePlaceablebyX()); 
			Placeable* midpoint = placeableList[int(placeableList.size()/2)];
			partitionPoint  = (XArray[int(XArray.size()/2) + 1] + XArray[int(XArray.size()/2)])/2;
			for(std::vector<Placeable*>::iterator i=placeableList.begin();i!=placeableList.end();++i){
				this->addPoint(*i, treenn);
			}

		}
		else{
			whichAxis = YAXIS;
			std::sort(placeableList.begin(),placeableList.end(),ComparePlaceablebyY()); 
			Placeable* midpoint = placeableList[int(placeableList.size()/2)];
			partitionPoint = (YArray[int(YArray.size()/2) + 1] + YArray[int(YArray.size()/2)])/2;
			for(std::vector<Placeable*>::iterator i=placeableList.begin();i!=placeableList.end();++i){
				this->addPoint(*i, treenn);
			}

		}
		placeableList.clear();
	}

	void AnnSpace::deletePoint(Placeable* placeable, TreeNNSpatialSearch* treenn){
		if(this->isLeaf()){
			std::vector<Placeable*>::iterator result = std::find(placeableList.begin(), placeableList.end(), placeable);
			placeableList.erase(result);
			if(placeableList.size() < treenn->pointLowerThreshold && parent != NULL){
				this -> mergeSpace(treenn);
			}
		}
		else{
			Vector3f location = placeable->getLocation();
			child[chooseChild(location.x, location.y)] -> deletePoint(placeable, treenn);
		}
	}


	void AnnSpace::addPoint(Placeable* placeable, TreeNNSpatialSearch* treenn){
		if(this->isLeaf()){
			placeableList.push_back(placeable);
			if(placeableList.size() + 1 > treenn->pointUpperThreshold){
				this->partitionSpace(treenn);
			}
		}
		else{		
			Vector3f location = placeable->getLocation();
			child[chooseChild(location.x, location.y)] -> addPoint(placeable, treenn);
		}
	}


	Placeable* AnnSpace::findNN(float x, float y, Placeable* exclude){	
		if(this->isLeaf()){
			Vector3f queryPoint;
			queryPoint.x = x;
			queryPoint.y = y;
			queryPoint.z = 0;
			float maxDistance;
			Placeable * maxDistanceItem=NULL;
			for(std::vector<Placeable*>::iterator i=placeableList.begin();i!=placeableList.end();++i) {
				Placeable * current=*i;//find out what's "inside" i
				float currentDistance=(current->getLocation()-queryPoint).length();
				if ((maxDistanceItem==NULL || currentDistance<maxDistance) && current != exclude){
					maxDistance=currentDistance;
					maxDistanceItem=current;
				}
			}
			return maxDistanceItem;
		}
		else{
			return child[chooseChild(x, y)] -> findNN(x, y, exclude);
		}
	}

	void AnnSpace::mergeSpace(TreeNNSpatialSearch* treenn){

		//Get the remaining points in this list
		std::vector<Placeable*> newPlaceableList;
		newPlaceableList = placeableList;

		//if this is the case, we are adjacent to the root
		if(parent->parent == NULL){
			AnnSpace* sibling = parent->findSibling(this);
			sibling->setParent(NULL);
			treenn->reassignRoot(sibling);
			for(std::vector<Placeable*>::iterator i=newPlaceableList.begin();i!=newPlaceableList.end();++i) {
				sibling->addPoint(*i, treenn);
			}
		}

		else{
			AnnSpace* sibling = parent->findSibling(this);
			AnnSpace* grandParent = parent->parent;

			//set sibling parent to grandparent
			sibling->setParent(grandParent);

			//set grandparent child to the sibling in place of the parent
			grandParent->resetChild(parent, sibling);

			//now go back and add the points that were remaining in the node that was deleted
			for(std::vector<Placeable*>::iterator i=newPlaceableList.begin();i!=newPlaceableList.end();++i) {
					grandParent->addPoint(*i, treenn);
			}
		}
		delete this;
	}






		/*
		if(child[0] != NULL && child[1] != NULL){
			//First find whether both children are leaves
			if(child[0]->isLeaf() && child[1]->isLeaf()){
				partitionPoint = 0.0f;
				std::vector<Placeable*> newPlaceableList;
				newPlaceableList = child[0]->getChildList();
				for(std::vector<Placeable*>::iterator i=newPlaceableList.begin();i!=newPlaceableList.end();++i) {
					Placeable* current=*i;
					this->placeableList.push_back(current);
				}
				newPlaceableList = child[1]->getChildList();
				for(std::vector<Placeable*>::iterator i=newPlaceableList.begin();i!=newPlaceableList.end();++i) {
					Placeable* current=*i;
					this->placeableList.push_back(current);
				}
				delete child[0];
				delete child[1];
				child[0] = NULL;
				child[1] = NULL;
			}
		//in this case, you want to find the sib
		
/*
			//If one child is not a leaf, 
			else{
				AnnSpace* departingChild;
				AnnSpace* remainingChild;
				float newPartition;
				if(child[0]->isLeaf()){
					newPartition = FLT_MAX;
					departingChild = child[0];				
					remainingChild = child[1];
					child[0] = NULL;
				}
				else{
					float newPartition = FLT_MIN;
					departingChild = child[1];
					remainingChild = child[0];
					child[1] = NULL;
				}
				remainingChild->setPartition(newPartition);
				std::vector<Placeable*> newPlaceableList;
				newPlaceableList = departingChild->getChildList();
				for(std::vector<Placeable*>::iterator i=newPlaceableList.begin();i!=newPlaceableList.end();++i) {
					Placeable* current=*i;
					remainingChild->placeableList.push_back(current);
				}
				delete departingChild;
			}
			*/
		/*
		}
		else{
			for(int i=0; i<2; i++){
				if(child[i] != NULL){
					std::vector<Placeable*> newPlaceableList;
					newPlaceableList = child[i]->getChildList();
					for(std::vector<Placeable*>::iterator j=newPlaceableList.begin();j!=newPlaceableList.end();++j) {
						Placeable* current=*j;
						this->placeableList.push_back(current);
					}
					delete child[i];
					child[i] = NULL;
				}
			}
			parent->mergeSpace(treenn);
		}
	}
	*/

	std::vector<Placeable*> AnnSpace::getChildList(){
		return placeableList;
	}

	//Given one of the two children, return the other child
	AnnSpace* AnnSpace::findSibling(AnnSpace* offspring){
		if(child[0] != offspring){
			return child[0];
		}
		else{
			return child[1];
		}
	}

	void AnnSpace::resetChild(AnnSpace* departingChild, AnnSpace* newChild){
		for(int i=0;i<2;i++){
			if(child[i] == departingChild){
				child[i] = newChild;
			}
		}
	}
		



		
}
