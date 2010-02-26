#include "Platform.hpp"
#include "genome.pb.h"
#include "CellComponent.hpp"
#include "ActivityStats.hpp"
#include "Neuron.hpp"
#include "Branch.hpp"
#include "Synapse.hpp"
int main() {
    Elysia::Vector3f test(0,1,2);
    std::tr1::unordered_map<Elysia::String,Elysia::Vector3f> bleh;
    bleh["ahoy"]=test;
    Elysia::Genome::Genome genes;
	//std::vector<Branch *>BranchestoWipe;
	int GLOBAL_TIME = 0;					//timestamp for sim events

	std::vector<Neuron *>neuronList;


	//Generate Neurons have to touch all neurons in development and all neurons firing
	//Have to touch all synapses, could only touch all active synapses with a vector


	return 0;
}
