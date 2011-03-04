#include "AnnSpace.hpp"


namespace Elysia{


	AnnSpace::AnnSpace(){
		child[0] = NULL;
		child[1] = NULL;
		parent = NULL;
	}

	bool AnnSpace::isLeaf(){
		if (child[0] == NULL){
			assert(child[1] == NULL);
			return true;
		}
		else{
			return false;
		}
	}

	int AnnSpace::chooseChild(float x, float y){
		if(whichAxis == XAXIS){
			if(x > partitionPoint){return 1;}
			else{return 0;}
		}
		else{
			if(y  > partitionPoint){return 1;}
			else{return 0;}
		}
	}

	class CompareNeuronbyY {
		public:
		bool operator() (const Neuron*a, const Neuron*b) const {
		return a->getLocation().y<b->getLocation().y;
		}
	};
	class CompareNeuronbyX {
		public:
		bool operator() (const Neuron*a, const Neuron*b) const {
		return a->getLocation().x<b->getLocation().x;
		}
	};

	void AnnSpace::partitionSpace(STANNSpatialSearch* stann){
		std::vector<float> XArray;
		std::vector<float> YArray;
		for(std::vector<Neuron*>::iterator i=neuronList.begin();i!=neuronList.end();++i){
			Neuron* current=*i;
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
		float Xdist = XArray[int(XArray.size()/2) + 1] - XArray[int(XArray.size())];
		float Ydist = XArray[int(YArray.size()/2) + 1] - YArray[int(YArray.size())];
		
		if(Xdist > Ydist){
			class CompareNeuronbyX {
				public:
				bool operator() (const Neuron*a, const Neuron*b) const {
				return a->getLocation().x<b->getLocation().x;
				}
			};
			
			std::sort(neuronList.begin(),neuronList.end(),CompareNeuronbyX()); 
			Neuron* midpoint = neuronList[int(neuronList.size()/2)];
			float partition = (XArray[int(XArray.size()/2) + 1] + XArray[int(XArray.size())])/2;
			
			for(std::vector<Neuron*>::iterator i=neuronList.begin();i!=neuronList.begin()+neuronList.size()/2;++i){

				child[0]->addPoint(*i, stann);
			}
			for(std::vector<Neuron*>::iterator i=neuronList.begin()+neuronList.size()/2;i!=neuronList.end();++i){
				child[1]->addPoint(*i, stann);
			}
		}
		else{
			
			std::sort(neuronList.begin(),neuronList.end(),CompareNeuronbyY()); 
			Neuron* midpoint = neuronList[int(neuronList.size()/2)];
			
			float partition = (YArray[int(YArray.size()/2) + 1] + YArray[int(YArray.size())])/2;
			
			for(std::vector<Neuron*>::iterator i=neuronList.begin();i!=neuronList.begin()+neuronList.size()/2;++i){
				child[0]->addPoint(*i, stann);
			}
			for(std::vector<Neuron*>::iterator i=neuronList.begin()+neuronList.size()/2;i!=neuronList.end();++i){
				child[1]->addPoint(*i, stann);
			}
		}
		neuronList.clear();
	}

	void AnnSpace::deletePoint(Neuron* neuron, STANNSpatialSearch* stann){
		if(this->isLeaf()){
			if(neuronList.size() > stann->pointLowerThreshold+1){
				std::vector<Neuron *>::iterator result = std::find(neuronList.begin(), neuronList.end(), neuron);
				neuronList.erase(result);
			}
			else{
				

				//DETERMINE MERGE POINT
				//MERGE SPACE
			}
		}
		else{
			Vector3f location = neuron->getLocation();
			child[chooseChild(location.x, location.y)] -> addPoint(neuron, stann);
		}
	}

	void AnnSpace::addPoint(Neuron* neuron, STANNSpatialSearch* stann){
		if(this->isLeaf()){
			neuronList.push_back(neuron);
			if(stann->pointUpperThreshold > neuronList.size() + 1){
				//PARTITION
			}
		}
		else{		
			Vector3f location = neuron->getLocation();
			child[chooseChild(location.x, location.y)] -> addPoint(neuron, stann);
		}
	}


	Neuron* AnnSpace::findNN(float x, float y, Neuron* exclude){	
		if(this->isLeaf()){
			Vector3f queryPoint;
			queryPoint.x = x;
			queryPoint.y = y;
			queryPoint.z = 0;
			float maxDistance;
			Neuron * maxDistanceItem=NULL;
			for(std::vector<Neuron*>::iterator i=neuronList.begin();i!=neuronList.end();++i) {
				Neuron * current=*i;//find out what's "inside" i
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

	void AnnSpace::mergeSpace(){}

	
}
