#include "Platform.hpp"
#include "genome.pb.h"
#include "CellComponent.hpp"
#include "ActivityStats.hpp"
#include "Neuron.hpp"
#include "Branch.hpp"
#include "Synapse.hpp"
#include "Base64.hpp"
#include "Brain.hpp"
#include "ProteinEnvironment.hpp"

int runtest(){

    ProteinEnvironment dummy = new ProteinEnvironment();

	Brain *brain= new Brain(*dummy);
	for(int i=0;i<100;i++){
		Vector3f v;
		v.x = i;
		v.y = i;
		v.z = i;
		Neuron *n;
		brain->mAllNeurons.insert(n = new Neuron(brain, 2, 3, 4, v); 
		//const Vector3f &location):  mNeuronLocation(location){));
	}
	return 1;
}