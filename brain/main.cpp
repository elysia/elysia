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
#include "MainThread.hpp"
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
void nilDestroy() {}
Elysia::SharedLibrary gVis(Elysia::SharedLibrary::prefix()+"vis"+Elysia::SharedLibrary::postfix()+Elysia::SharedLibrary::extension());
typedef std::map<std::string,std::tr1::shared_ptr<Elysia::SharedLibrary> > DevelopmentPluginMap;
DevelopmentPluginMap gPlugins;
bool loadedVis=false;

void destroyDevelopmentPlugins() {
    void (*destroy)()=&nilDestroy;
    for (DevelopmentPluginMap::iterator i=gPlugins.begin(),ie=gPlugins.end();i!=ie;++i) {
        destroy=(void(*)())i->second->symbol("destroy");
        if (destroy)
            (*destroy)();            
    }

}

int asyncMain(int argc, char**argv, bool loadvis) {
    void (*destroy)()=&nilDestroy;
    if (loadvis) {
      if (!loadedVis) {
        std::cerr<<"Failed to load vis library\n";
      }else {
        void (*init)();
        init=(void(*)())gVis.symbol("init");
        destroy=(void(*)())gVis.symbol("destroy");
        (*init)();
        for (DevelopmentPluginMap::iterator i=gPlugins.begin(),ie=gPlugins.end();i!=ie;++i) {
            init=(void(*)())i->second->symbol("init");
            if (init)
                (*init)();            
        }
      }
    }
    
    Elysia::Vector3f test(0,1,2);
    std::tr1::unordered_map<Elysia::String,Elysia::Vector3f> bleh;
    bleh["ahoy"]=test;
    Elysia::Genome::Genome genes;
    if (argc>1) {
		if(0 == strcmp(argv[1],"-test")){
			int retval= runtest();
            if (destroy)
                (*destroy)();
            destroyDevelopmentPlugins();
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
    for (size_t i=0;i<1000;++i) {
        brain.tick();
    }
    

	
    (*destroy)();
    destroyDevelopmentPlugins();
	

	return 0;
}
void asyncMainWrapper(int argc, char**argv, bool loadvis) {
    asyncMain(argc,argv,loadvis);
}
void loadDevelLib(const char*name){
    std::tr1::shared_ptr<Elysia::SharedLibrary> item(new Elysia::SharedLibrary(
                                                         Elysia::SharedLibrary::prefix()+name
                                                         +Elysia::SharedLibrary::postfix()+Elysia::SharedLibrary::extension()));
    gPlugins[name]=item;
}
int main(int argc, char **argv) {
    bool loadvis=true;
    loadDevelLib("naive");
    for (int i=0;i<argc;++i) {
      if (strcmp(argv[i],"-nogfx")==0) {
        loadvis=false;
      }else if (strncmp(argv[i],"-plugin=",9)==0) {
          loadDevelLib(argv[i]+9);
      }
    }
    if (loadvis)
      loadedVis =gVis.load();
    std::vector<std::string> failedPlugins;
    for (DevelopmentPluginMap::iterator i=gPlugins.begin(),ie=gPlugins.end();i!=ie;++i) {
        if (!i->second->load()) {
            failedPlugins.push_back(i->first);
        }
    }
    while(failedPlugins.size()) {
        gPlugins.erase(failedPlugins.back());
        failedPlugins.pop_back();
    }
    std::tr1::shared_ptr<boost::thread> formerMain;
    if (
#ifdef __APPLE__
	loadvis
#else
    false
#endif
        )

    {
        formerMain=Elysia::MainThread::giveUpMain(std::tr1::bind(asyncMainWrapper,argc,argv,true));
        
    }else return asyncMain(argc,argv,loadvis);
    return 0;
}
