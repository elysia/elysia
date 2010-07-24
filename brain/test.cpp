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
#include <time.h>

namespace Elysia {
void testTwoConnectedNeurons() {
    ProteinEnvironment *myProteinEnvironment= new SimpleProteinEnvironment();

	Brain *brain= new Brain(myProteinEnvironment);
	FILE *dendriteTree=NULL;
	dendriteTree = fopen("Dendritic_Tree.txt", "w");
	std::vector<Neuron *> createdList;
	int neuronNumber = 2;
	for(int i=0;i<neuronNumber;i++){
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
		srand(time(NULL));
		brain->mAllNeurons.insert(n = new Neuron(brain, 2, 3, 4, v,gene)); 
		createdList.push_back(n);
		}
	for(float i=0;i<neuronNumber;i++){
		Neuron *n = createdList[i];
        n->developSynapse(n->getActivityStats());       
        size_t parent;
        parent = 0;
        n->visualizeTree(dendriteTree, parent);
		//const Vector3f &location):  mNeuronLocation(location){));
	}
	for(int j=0; j<10; j++){
		for(float i=0;i<neuronNumber;i++){
			Neuron *n = createdList[i];
			if(j== 0 && i==0){n->activateComponent(*brain,100);}
			//if(j== 2){brain->inactivateNeuron(n);}
			n->tick();
		//const Vector3f &location):  mNeuronLocation(location){));
		}
		
		brain ->tick();
	}
	fclose(dendriteTree);
    delete brain;
}

void testDevelopment(){
	ProteinEnvironment *myProteinEnvironment= new SimpleProteinEnvironment();

	Brain *brain= new Brain(myProteinEnvironment);
	FILE *dendriteTree=NULL;
	dendriteTree = fopen("Development_Tree.txt", "w");
	std::vector<Neuron *> createdList;
	int neuronNumber = 3;

	for(int i=0;i<neuronNumber;i++){
        Genome::Gene gene;//FIXME set source and target regions to match the desired behavior
        Genome::TemporalBoundingBox *sourcebb=gene.add_bounds();
        Genome::TemporalBoundingBox *targetbb=gene.add_target_region();
        sourcebb->set_minx(50+i);
        sourcebb->set_miny(50+i);
        sourcebb->set_minz(50+i);

        sourcebb->set_maxx(150+i);
        sourcebb->set_maxy(150+i);
        sourcebb->set_maxz(150+i);

        targetbb->set_minx(i);
        targetbb->set_miny(i);
        targetbb->set_minz(i);

        targetbb->set_maxx(i+1);
        targetbb->set_maxy(i+1);
        targetbb->set_maxz(i+1);

		Vector3f v;
		v.x = i;
		v.y = i;
		v.z = i;
		Neuron *n;
		srand(time(NULL));
		brain->mAllNeurons.insert(n = new Neuron(brain, 2, 3, 4, v,gene)); 
		createdList.push_back(n);
	}

		for(int i=100;i<100+neuronNumber;i++){
        Genome::Gene gene;//FIXME set source and target regions to match the desired behavior
        Genome::TemporalBoundingBox *sourcebb=gene.add_bounds();
        Genome::TemporalBoundingBox *targetbb=gene.add_target_region();
        sourcebb->set_minx(0);
        sourcebb->set_miny(0);
        sourcebb->set_minz(0);

        sourcebb->set_maxx(20);
        sourcebb->set_maxy(20);
        sourcebb->set_maxz(20);

        targetbb->set_minx(i);
        targetbb->set_miny(i);
        targetbb->set_minz(i);

        targetbb->set_maxx(i+1);
        targetbb->set_maxy(i+1);
        targetbb->set_maxz(i+1);

        

		Vector3f v;
		v.x = i;
		v.y = i;
		v.z = i;
		Neuron *n;
		srand(time(NULL));
		brain->mAllNeurons.insert(n = new Neuron(brain, 2, 3, 4, v,gene)); 
		createdList.push_back(n);
	}

	for(float i=0;i<2*neuronNumber;i++){
		Neuron *n = createdList[i];
        n->developSynapse(n->getActivityStats());       
        size_t parent;
        parent = 0;
        n->visualizeTree(dendriteTree, parent);
		//const Vector3f &location):  mNeuronLocation(location){));
	}
	for(int j=0; j<10; j++){
		for(float i=0;i<neuronNumber;i++){
			Neuron *n = createdList[i];
			if(j==0 && i==0){n->activateComponent(*brain,100);}
			n->tick();
		//const Vector3f &location):  mNeuronLocation(location){));
		}
		
		brain ->tick();
	}
	fclose(dendriteTree);
    delete brain;
}




void testResultHelper(const std::vector<std::pair<Elysia::Genome::Effect, float> >&combinedResult, float*grow_leaf_count,float*grow_neuron_count,float*other_count){
   using namespace Elysia::Genome;
    *grow_neuron_count=0;
    *grow_leaf_count=0;
    *other_count=0;
    for (size_t i=0;i<combinedResult.size();++i) {
       switch(combinedResult[i].first) {
         case GROW_NEURON:
           *grow_neuron_count+=combinedResult[i].second;
           break;
         case GROW_LEAF:
           *grow_leaf_count+=combinedResult[i].second;
           break;
         default:
           *other_count+=combinedResult[i].second;
           break;
       }
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
   firstAuxProtein.set_protein_code(GROW_LEAF);//so we see that they are additive
   firstAuxProtein.set_density(0.25);
   *firstGene.add_external_proteins()=firstProtein;
   assert(firstGene.external_proteins(0).protein_code()==GROW_NEURON);
   *firstGene.add_external_proteins()=firstAuxProtein;
   assert(firstGene.external_proteins(1).protein_code()==GROW_LEAF);
   Elysia::Genome::TemporalBoundingBox firstRegion;
   firstRegion.set_minx(0);
   firstRegion.set_miny(0);
   firstRegion.set_minz(0);
   firstRegion.set_maxx(2);
   firstRegion.set_maxy(2);
   firstRegion.set_maxz(2);

   *firstGene.add_bounds()=firstRegion;

   Elysia::Genome::TemporalBoundingBox firstTargetRegion;
   firstTargetRegion.set_minx(5);
   firstTargetRegion.set_miny(5);
   firstTargetRegion.set_minz(5);
   firstTargetRegion.set_maxx(8);
   firstTargetRegion.set_maxy(8);
   firstTargetRegion.set_maxz(8);

   *firstGene.add_target_region()=firstTargetRegion;
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

   Elysia::Genome::TemporalBoundingBox secondTargetRegion;
   secondTargetRegion.set_minx(-5);
   secondTargetRegion.set_miny(-5);
   secondTargetRegion.set_minz(-5);
   secondTargetRegion.set_maxx(3);
   secondTargetRegion.set_maxy(3);
   secondTargetRegion.set_maxz(3);

   *secondGene.add_target_region()=secondTargetRegion;


   *father->add_genes()=firstGene;
   *father->add_genes()=secondGene;
   
   ProteinEnvironment * pe=new SimpleProteinEnvironment;
   pe->initialize(twoOverlappingGenes);
   std::vector<std::pair<Elysia::Genome::Effect, float> > combinedResult=pe->getCompleteProteinDensity(Vector3f(.5,.5,0));
   //check that firstResult matches expectations
   std::vector<std::pair<Elysia::Genome::Effect, float> > firstResult=pe->getCompleteProteinDensity(Vector3f(1.5,1.5,0));
   //check that secondResult matches expectations
   std::vector<std::pair<Elysia::Genome::Effect, float> > secondResult=pe->getCompleteProteinDensity(Vector3f(-.5,-.5,0));
   float grow_leaf_count=0;
   float grow_neuron_count=0;
   float other_count=0;

   testResultHelper(combinedResult,&grow_leaf_count,&grow_neuron_count,&other_count);
   assert(grow_leaf_count==.75);
   assert(grow_neuron_count==.125);
   assert(other_count==0);

   testResultHelper(firstResult,&grow_leaf_count,&grow_neuron_count,&other_count);
   assert(grow_leaf_count==.25);
   assert(grow_neuron_count==.125);
   assert(other_count==0);

   testResultHelper(secondResult,&grow_leaf_count,&grow_neuron_count,&other_count);
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
   assert(grow_neuron_gene->target_region(0).minx()==5);

   assert(combined_grow_leaf_gene->target_region(0).minx()==5||combined_grow_leaf_gene->target_region(0).minx()==-5);
   assert(first_grow_leaf_gene->target_region(0).minx()==5);
   assert(second_grow_leaf_gene->target_region(0).minx()==-5);

   delete pe;
}

}

int runtest(){
    //Elysia::testTwoConnectedNeurons();
	Elysia::testDevelopment();
    //Elysia::testProteinEnvironment();
    if (0) for (int i=0;i<30000;++i) {
        //Elysia::Brain b(new Elysia::SimpleProteinEnvironment);
//        usleep(10);        
    }
	//getchar();
	return 1;
	
}
