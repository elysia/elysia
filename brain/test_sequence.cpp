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
#include "Development.hpp"
#include "SimpleSpatialSearch.hpp"
#include <time.h>

typedef std::vector<Elysia::EffectAndTypeAndDensity> CombinedResults;

namespace Elysia {

void testTwoConnectedNeurons_seq() {
    ProteinEnvironment *myProteinEnvironment= new SimpleProteinEnvironment();

	Brain *brain= new Brain(myProteinEnvironment, new SimpleSpatialSearch);
	FILE *dendriteTree=NULL;
	dendriteTree = fopen("Dendritic_Tree.txt", "w");
	std::vector<Neuron *> createdList;
	int neuronNumber = 2;
	

	for(int j=0;j<neuronNumber;j++){
        float i=(float)j;
        Genome::Gene gene;//FIXME set source and target regions to match the desired behavior
        Genome::TemporalBoundingBox *sourcebb=gene.add_bounds();
        Genome::TemporalBoundingBox *targetbb=gene.add_bounds();
        sourcebb->set_minx(i);
        sourcebb->set_miny(i);
        sourcebb->set_minz(i);

        sourcebb->set_maxx(i);
        sourcebb->set_maxy(i);
        sourcebb->set_maxz(i);

        targetbb->set_minx(i);
        targetbb->set_miny(i);
        targetbb->set_minz(i);

        targetbb->set_maxx(1-i);
        targetbb->set_maxy(1-i);
        targetbb->set_maxz(2);

		Vector3f v;
		v.x = i;
		v.y = i;
		v.z = 1;
		Neuron *n;
		brain->mAllNeurons.insert(n = new Neuron(brain, 2, 3, 4, 2, 2, v,gene, 2, NULL)); 
		createdList.push_back(n);
	} // for

	for(int i=0;i<neuronNumber;i++){
		Neuron *n = createdList[i];
        n->development()->develop();       
        size_t parent;
        parent = 0;
        n->visualizeTree(dendriteTree, parent);
		//const Vector3f &location):  mNeuronLocation(location){));
	}
	for(int j=0; j<10; j++){
		for(int i=0;i<neuronNumber;i++){
			Neuron *n = createdList[i];
			if(j== 0 && i==0)
				n->activateComponent(*brain,100);
			//if(j== 2){brain->inactivateNeuron(n);}
			n->tick();
		//const Vector3f &location):  mNeuronLocation(location){));
		}
		
		brain ->tick();
	}
	fclose(dendriteTree);
    delete brain;
}


//Target is axon, Source is dendrite (?)
Neuron* placeTestSeqNeuron(Brain* brain, float locx, float locy, float locz, float sx, float sy, float sz, float range){
	
	float random = rand()/(float)RAND_MAX;	// some random position offset
	Genome::Gene gene;						// FIXME set source and target regions to match the desired behavior
    Genome::TemporalBoundingBox *sourcebb=gene.add_bounds();
    Genome::TemporalBoundingBox *targetbb=gene.add_dendrite_region();
    
	sourcebb->set_minx(sx-range*random);
    sourcebb->set_miny(sy-range*random);
    sourcebb->set_minz(1);

    sourcebb->set_maxx(sx+range*random);
    sourcebb->set_maxy(sy+range*random);
    sourcebb->set_maxz(1);

    targetbb->set_minx(locx-range);
    targetbb->set_miny(locy-range);
    targetbb->set_minz(1);

    targetbb->set_maxx(locx+range);
    targetbb->set_maxy(locy+range);
    targetbb->set_maxz(1);

	Vector3f v;
	v.x = locx+range*random;
	v.y = locy+range*random;
	v.z = 1;
	Neuron *n;
	brain->mAllNeurons.insert(n = new Neuron(brain, 2, 3, 4, 2, 2, v,gene, 2, NULL));
	return n;
}

void testSeqDevelopment(int neuronNumber){

	ProteinEnvironment *myProteinEnvironment = new SimpleProteinEnvironment();// generate new protein environment
	Brain *brain= new Brain(myProteinEnvironment, new SimpleSpatialSearch);	// new brain defined from this protein environment
	std::vector<Neuron *> createdList;				// a list of neurons created during the test
	FILE *dendriteTree=NULL;						// output dendrite tree
	Neuron *n;										// neurons created during the test sequence
	size_t parent;
	
	dendriteTree = fopen("Development_Tree.txt", "w");
	srand((unsigned int)time(NULL));
	
	//Region 1
	for(int i=0;i<neuronNumber;i++)
		createdList.push_back(placeTestSeqNeuron(brain, 10, 10, 1, 100, 100, 1, 5));

	//Region 2
	for(int i=0;i<neuronNumber;i++)
		createdList.push_back(placeTestSeqNeuron(brain, 100, 100, 1, 10, 10, 1, 5)); 

	for(int i=0;i<2*neuronNumber;i++){
		n = createdList[i];
        n->development()->develop(); 
        parent = 0;		// this seems a bit pointless
        n->visualizeTree(dendriteTree, parent);
		//const Vector3f &location):  mNeuronLocation(location){));
	}
	for(int j=0; j<10; j++){
		for(int i=0;i<neuronNumber;i++){
			n = createdList[i];
			if(j==0)
				n->activateComponent(*brain,100);
			n->tick();
		//const Vector3f &location):  mNeuronLocation(location){));
		}
		brain ->tick();
	}
	fclose(dendriteTree);
    delete brain;
}


void testSeqResultHelper(const CombinedResults &combinedResult, float*grow_leaf_count,
						 float*grow_neuron_count,float*other_count){
   using namespace Elysia::Genome;
    *grow_neuron_count=0;
    *grow_leaf_count=0;
    *other_count=0;
    for (size_t i=0;i<combinedResult.size();++i) {
       switch(combinedResult[i].effect) {
         case GROW_NEURON:
           *grow_neuron_count+=combinedResult[i].density;
           break;
         case GROW_LEAF:
           *grow_leaf_count+=combinedResult[i].density;
           break;
         default:
           *other_count+=combinedResult[i].density;
           break;
       }
   }

}

void testSeqProteinEnvironment() {
   using namespace Elysia::Genome;
   Elysia::Genome::Genome twoOverlappingGenes;
   Elysia::Genome::Chromosome *father=twoOverlappingGenes.mutable_fathers();
   Elysia::Genome::Gene firstGene;
   Elysia::Genome::Protein firstProtein;
   firstProtein.set_protein_type(GROW_NEURON);//so we can easily identify where
   firstProtein.set_protein_effect(GROW_NEURON);//so we can easily identify where
   firstProtein.set_density(0.125);   
   Elysia::Genome::Protein firstAuxProtein;
   firstAuxProtein.set_protein_type(GROW_LEAF);//so we see that they are additive
   firstAuxProtein.set_protein_effect(GROW_LEAF);//so we see that they are additive
   firstAuxProtein.set_density(0.25);
   *firstGene.add_external_proteins()=firstProtein;
   assert(firstGene.external_proteins(0).protein_effect()==GROW_NEURON);
   assert(firstGene.external_proteins(0).protein_type()==GROW_NEURON);
   *firstGene.add_external_proteins()=firstAuxProtein;
   assert(firstGene.external_proteins(1).protein_type()==GROW_LEAF);
   assert(firstGene.external_proteins(1).protein_effect()==GROW_LEAF);
   Elysia::Genome::TemporalBoundingBox firstRegion;
   firstRegion.set_minx(0);
   firstRegion.set_miny(0);
   firstRegion.set_minz(0);
   firstRegion.set_maxx(2);
   firstRegion.set_maxy(2);
   firstRegion.set_maxz(2);

   *firstGene.add_bounds()=firstRegion;

   Elysia::Genome::TemporalBoundingBox firstDendriteRegion;
   firstDendriteRegion.set_minx(5);
   firstDendriteRegion.set_miny(5);
   firstDendriteRegion.set_minz(5);
   firstDendriteRegion.set_maxx(8);
   firstDendriteRegion.set_maxy(8);
   firstDendriteRegion.set_maxz(8);

   *firstGene.add_dendrite_region()=firstDendriteRegion;
   Elysia::Genome::Gene secondGene;
   Elysia::Genome::Protein secondProtein;
   secondProtein.set_protein_type(GROW_LEAF);
   secondProtein.set_protein_effect(GROW_LEAF);
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

   Elysia::Genome::TemporalBoundingBox secondDendriteRegion;
   secondDendriteRegion.set_minx(-5);
   secondDendriteRegion.set_miny(-5);
   secondDendriteRegion.set_minz(-5);
   secondDendriteRegion.set_maxx(3);
   secondDendriteRegion.set_maxy(3);
   secondDendriteRegion.set_maxz(3);

   *secondGene.add_dendrite_region()=secondDendriteRegion;


   *father->add_genes()=firstGene;
   *father->add_genes()=secondGene;
   
   ProteinEnvironment * pe=new SimpleProteinEnvironment;
   pe->initialize(twoOverlappingGenes);
   std::vector<EffectAndTypeAndDensity > combinedResult=pe->getCompleteProteinDensity(Vector3f(.5,.5,0));
   //check that firstResult matches expectations
   std::vector<EffectAndTypeAndDensity > firstResult=pe->getCompleteProteinDensity(Vector3f(1.5,1.5,0));
   //check that secondResult matches expectations
   std::vector<EffectAndTypeAndDensity > secondResult=pe->getCompleteProteinDensity(Vector3f(-.5,-.5,0));
   float grow_leaf_count=0;
   float grow_neuron_count=0;
   float other_count=0;

   testSeqResultHelper(combinedResult,&grow_leaf_count,&grow_neuron_count,&other_count);
   assert(grow_leaf_count==.75);
   assert(grow_neuron_count==.125);
   assert(other_count==0);

   testSeqResultHelper(firstResult,&grow_leaf_count,&grow_neuron_count,&other_count);
   assert(grow_leaf_count==.25);
   assert(grow_neuron_count==.125);
   assert(other_count==0);

   testSeqResultHelper(secondResult,&grow_leaf_count,&grow_neuron_count,&other_count);
   assert(grow_leaf_count==.5);
   assert(grow_neuron_count==0);
   assert(other_count==0);
   //return;//UNCOMMENT TO GET FAILING TEST
   const Gene * grow_neuron_gene = &pe->retrieveGene(Vector3f(0.5,0.5,0),
                                              GROW_NEURON);
   const Gene * combined_grow_leaf_gene = &pe->retrieveGene(Vector3f(0.5,0.5,0),
                                                     GROW_LEAF);
   const Gene * first_grow_leaf_gene = &pe->retrieveGene(Vector3f(1.5,1.5,0),
                                                  GROW_LEAF);
   const Gene * second_grow_leaf_gene = &pe->retrieveGene(Vector3f(-0.5,-0.5,0),
                                                   GROW_LEAF);
   assert(grow_neuron_gene->dendrite_region(0).minx()==5);

   assert(combined_grow_leaf_gene->dendrite_region(0).minx()==5||combined_grow_leaf_gene->dendrite_region(0).minx()==-5);
   assert(first_grow_leaf_gene->dendrite_region(0).minx()==5);
   assert(second_grow_leaf_gene->dendrite_region(0).minx()==-5);

   delete pe;
}

}

int runtestsequence(){
    //Elysia::testTwoConnectedNeurons();
    Elysia::testSeqProteinEnvironment();
	Elysia::testSeqDevelopment(4);
    if (0) for (int i=0;i<30000;++i) {
		Elysia::Brain b(new Elysia::SimpleProteinEnvironment, new Elysia::SimpleSpatialSearch);
//        usleep(10);        
    }
	//getchar();
	return 1;
	
}
