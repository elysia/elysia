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

	void AnnSpace::partitionSpace(){}

	void AnnSpace::deletePoint(Neuron* neuron, STANNSpatialSearch* stann){
		if(this->isLeaf()){
			if(neuronList.size() > stann->pointLowerThreshold+1){
				std::vector<Neuron *>::iterator result = std::find(neuronList.begin(), neuronList.end(), neuron);
				neuronList.erase(result);
			}
			else{
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