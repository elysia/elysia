#include "Platform.hpp"
#include "genome.pb.h"
#include "CellComponent.hpp"
#include "ActivityStats.hpp"
#include "Neuron.hpp"
#include "Branch.hpp"
#include "Synapse.hpp"
#include "Base64.hpp"

bool loadFile(const char* fileName, Elysia::Genome::Genome &retval) {
    FILE * fp=fopen(fileName,"rb");
    if (!fp) return false;
    fseek(fp,0,SEEK_END);
    size_t fileSize=ftell(fp);
    void * data = malloc(fileSize);
    fseek(fp,0,SEEK_SET);
    fread(data,1,fileSize,fp);
    fclose(fp);
    std::vector<unsigned char> buffer;
    if (fromBase64(buffer,Base64::MemoryReference(data,fileSize))) {
        if (buffer.size()&&retval.ParseFromArray(&*buffer.begin(),buffer.size())) {
            return true;
        }else {
            return retval.ParseFromArray(data,fileSize);
        }
    }else {
        return retval.ParseFromArray(data,fileSize);
    }
}

int main(int argc, char **argv) {
    Elysia::Vector3f test(0,1,2);
    std::tr1::unordered_map<Elysia::String,Elysia::Vector3f> bleh;
    bleh["ahoy"]=test;
    Elysia::Genome::Genome genes;
    if (argc>1) {        
        bool retval=loadFile(argv[1],genes);
        if (!retval) {
            printf("Error loading saved file\n");            
        }else {
            printf("Success loading saved file\n");
        }
    }
	//std::vector<Branch *>BranchestoWipe;

    

	//Generate Neurons have to touch all neurons in development and all neurons firing
	//Have to touch all synapses, could only touch all active synapses with a vector


	return 0;
}
