#include "Platform.hpp"
#include "genome.pb.h"
#include "CellComponent.hpp"
#include "ActivityStats.hpp"
#include "Neuron.hpp"
#include "Branch.hpp"
#include "Synapse.hpp"
#include "Base64.hpp"
#include "Brain.hpp"
#include "SimpleProteinEnvironment.hpp"
#include "genome.pb.h"
#include <time.h>

namespace Elysia {
void testTwoConnectedNeurons() {
    ProteinEnvironment *myProteinEnvironment= new SimpleProteinEnvironment();

	Brain *brain= new Brain(myProteinEnvironment);
	FILE *dendriteTree=NULL;
	dendriteTree = fopen("Dendritic_Tree.txt", "w");
	for(float i=0;i<2;i++){
        Genome::Gene gene;//FIXME set source and target regions to match the desired behavior
        Genome::TemporalBoundingBox *sourcebb=gene.add_bounds();
        Genome::TemporalBoundingBox *targetbb=gene.add_bounds();
        sourcebb->set_minx(i);
        sourcebb->set_miny(i);
        sourcebb->set_minz(i);

        sourcebb->set_maxx(i);
        sourcebb->set_maxy(i);
        sourcebb->set_maxz(i);

        targetbb->set_minx(1-i);
        targetbb->set_miny(1-i);
        targetbb->set_minz(1-i);

        targetbb->set_maxx(1-i);
        targetbb->set_maxy(1-i);
        targetbb->set_maxz(1-i);

        

		Vector3f v;
		v.x = i;
		v.y = i;
		v.z = i;
		Neuron *n;
		srand(time(NULL));
		brain->mAllNeurons.insert(n = new Neuron(brain, 2, 3, 4, v,gene)); 
        n->developSynapse(n->getActivityStats());
        
        size_t parent;
        parent = 0;
        n->visualizeTree(dendriteTree, parent);
        n->activateComponent(*brain,100);
        n->tick();
		//const Vector3f &location):  mNeuronLocation(location){));
	}

}

void testProteinEnvironment() {
   using namespace Elysia::Genome;
   Elysia::Genome::Genome twoOverlappingGenes;
   Elysia::Genome::Chromosome *father=twoOverlappingGenes.mutable_fathers();
   Elysia::Genome::Gene firstGene;
   Elysia::Genome::Protein firstProtein;
   firstProtein.set_protein_code(GROW_NEURON);//so we can easily identify where
   firstProtein.set_density(0.125);   
   Elysia::Genome::Protein firstAuxProtein;
   firstProtein.set_protein_code(GROW_LEAF);//so we see that they are additive
   firstProtein.set_density(0.25);
   *firstGene.add_external_proteins()=firstProtein;
   *firstGene.add_external_proteins()=firstAuxProtein;
   Elysia::Genome::TemporalBoundingBox firstRegion;
   firstRegion.set_minx(0);
   firstRegion.set_miny(0);
   firstRegion.set_minz(0);
   firstRegion.set_maxx(2);
   firstRegion.set_maxy(2);
   firstRegion.set_maxz(2);

   *firstGene.add_bounds()=firstRegion;
   Elysia::Genome::Gene secondGene;
   Elysia::Genome::Protein secondProtein;
   secondProtein.set_protein_code(GROW_LEAF);
   secondProtein.set_density(0.5);
   *secondGene.add_external_proteins()=secondProtein;
   Elysia::Genome::TemporalBoundingBox secondRegion;
   secondRegion.set_minx(-1);
   secondRegion.set_miny(-1);
   secondRegion.set_minz(-1);
   secondRegion.set_maxx(1);
   secondRegion.set_maxy(1);
   secondRegion.set_maxz(1);
   *secondGene.add_bounds()=secondRegion;
   *father->add_genes()=firstGene;
   *father->add_genes()=secondGene;
   
   ProteinEnvironment * pe=new SimpleProteinEnvironment;
   pe->initialize(twoOverlappingGenes);
   std::vector<std::pair<Elysia::Genome::Effect, float> > combinedResult=pe->getCompleteProteinDensity(Vector3f(.5,.5,.5));
   //check that firstResult matches expectations
   std::vector<std::pair<Elysia::Genome::Effect, float> > firstResult=pe->getCompleteProteinDensity(Vector3f(1.5,1.5,1.5));
   //check that secondResult matches expectations
   std::vector<std::pair<Elysia::Genome::Effect, float> > secondResult=pe->getCompleteProteinDensity(Vector3f(-.5,-.5,-.5));
   assert(combinedResult.size()==2);
   assert(firstResult.size()==2);
   assert(secondResult.size()==1);
   assert((combinedResult[0].first==GROW_LEAF&&combinedResult[0].second==.75)||
          (combinedResult[1].first==GROW_LEAF&&combinedResult[1].second==.75));
   assert((combinedResult[0].first==GROW_NEURON&&combinedResult[0].second==.125)||
          (combinedResult[1].first==GROW_NEURON&&combinedResult[1].second==.125));
   assert((firstResult[0].first==GROW_LEAF&&firstResult[0].second==.25)||
          (firstResult[1].first==GROW_LEAF&&firstResult[1].second==.25));
   assert((firstResult[0].first==GROW_NEURON&&firstResult[0].second==.125)||
          (firstResult[1].first==GROW_NEURON&&firstResult[1].second==.125));
   assert(secondResult[0].first==GROW_LEAF&&secondResult[0].second==.5);
   delete pe;
}
}

int runtest(){
    Elysia::testTwoConnectedNeurons();
    Elysia::testProteinEnvironment();
	getchar();
	return 1;
	
}
