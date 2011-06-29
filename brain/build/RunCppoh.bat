cd sirikata
cd Release
cppoh.exe --oh.plugin-search=../../Release:..:. --oh.plugins=weight-exp,oh-sqlite,oh-file,weight-sqr,tcpsst,weight-const,ogregraphics,colladamodels,nvtt,common-filters,csvfactory,restoreObjFactory,oh-file,oh-sqlite,scripting-js,simplecamera,brainvw --objecthost="--scriptManagers=vw:{},simplecamera:{},js:{--import-paths=../../../../externals/sirikata/liboh/plugins/js/scripts,../../../scripts} --simOptions=ogregraphics:{--search_path=../../../../externals/sirikata/libproxyobject/plugins/}" --object-factory-opts=--db=../../scene.db 

REM --default-script-opts=--init-script=../../../../externals/sirikata/liboh/plugins/js/scripts/std/default.em
