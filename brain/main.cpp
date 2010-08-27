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
#include "SharedLibrary.hpp"
#include "test.hpp"

bool loadFile(const char* fileName, Elysia::Genome::Genome &retval) {
    FILE * fp=fopen(fileName,"rb");
    if (!fp) return false;
    fseek(fp,0,SEEK_END);
    size_t fileSize=ftell(fp);
    std::vector<unsigned char> data(fileSize);
    fseek(fp,0,SEEK_SET);
    if (!data.empty()) {
        fread(&*data.begin(),1,fileSize,fp);
    }
    fclose(fp);
    if (data.empty()) return false;
    std::vector<unsigned char> buffer;
    if (fromBase64(buffer,Base64::MemoryReference(&*data.begin(),fileSize))) {
        if (buffer.size()&&retval.ParseFromArray(&*buffer.begin(),buffer.size())) {
            return true;
        }else {
            return retval.ParseFromArray(&*data.begin(),fileSize);
        }
    }else {
        return retval.ParseFromArray(&*data.begin(),fileSize);
    }
}

int main(int argc, char **argv) {
    bool loadvis=true;
    void (*destroy)()=NULL;
    for (int i=0;i<argc;++i) {
      if (strcmp(argv[i],"-nogfx")==0) {
        loadvis=false;
      }
    }
    if (loadvis) {
      Elysia::SharedLibrary vis(Elysia::SharedLibrary::prefix()+"vis"+Elysia::SharedLibrary::postfix()+Elysia::SharedLibrary::extension());
      if (!vis.load()) {
        std::cerr<<"Failed to load vis library\n";
      }else {
        void (*init)();
        init=(void(*)())vis.symbol("init");
        destroy=(void(*)())vis.symbol("destroy");
        (*init)();
      }
    }
    
    Elysia::Vector3f test(0,1,2);
    std::tr1::unordered_map<Elysia::String,Elysia::Vector3f> bleh;
    bleh["ahoy"]=test;
    Elysia::Genome::Genome genes;
    if (argc>1) {
		if(0 == strcmp(argv[1],"-test")){
			int retval= runtest();
            (*destroy)();
            return retval;
		}
        bool retval=loadFile(argv[1],genes);
        if (!retval) {
            printf("Error loading saved file\n");            
        }else {
            printf("Success loading saved file\n");
        }
    }
    Elysia::Brain brain(&(new Elysia::SimpleProteinEnvironment)->initialize(genes));
	//std::vector<Branch *>BranchestoWipe;

    

	
    (*destroy)();
    getchar();

	

	return 0;
}
