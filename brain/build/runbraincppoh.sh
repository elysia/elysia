#DYLD_LIBRARY_PATH=$PWD/../:$DYLD_LIBRARY_PATH  LD_LIBRARY_PATH=$PWD/../:$LD_LIBRARY_PATH  ./cppoh_d --oh.plugins=weight-exp,weight-sqr,tcpsst,sqlite,weight-const,ogregraphics,colladamodels,csvfactory,simplecamera,scripting-js,brainvw
cd ../../build
cd ../build
for symfile in ../../externals/sirikata/dependencies/lib/OGRE/*; do ln -s $symfile; done
cd sirikata
./cppoh_d --oh.plugin-search=$PWD/..:$PWD --oh.plugins=weight-exp,oh-sqlite,oh-file,weight-sqr,tcpsst,weight-const,ogregraphics,colladamodels,nvtt,common-filters,csvfactory,restoreObjFactory,oh-file,oh-sqlite,scripting-js,simplecamera,brainvw --objecthost='--scriptManagers=vw:{},simplecamera:{},js:{--import-paths=../../../externals/sirikata/liboh/plugins/js/scripts} --simOptions=ogregraphics:{--search_path=../../../externals/sirikata/libproxyobject/plugins/}' --object-factory-opts=--db=../scene.db 
# --default-script-opts=--init-script=../../../externals/sirikata/liboh/plugins/js/scripts/std/default.em




##pre VW script ## ./cppoh_d --oh.plugin-search=$PWD/.. --oh.plugins=weight-exp,weight-sqr,tcpsst,sqlite,weight-const,ogregraphics,colladamodels,csvfactory,simplecamera,scripting-js,brainvw --objecthost=--scriptManagers=simplecamera:{},js:{}

