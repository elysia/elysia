cd sirikata
cd Debug
cppoh_d.exe --oh.plugin-search=../../Debug:..:. --oh.plugins=weight-exp,oh-sqlite,oh-file,weight-sqr,tcpsst,weight-const,ogregraphics,colladamodels,nvtt,common-filters,csvfactory,restoreObjFactory,oh-file,oh-sqlite,scripting-js,simplecamera,brainvw --objecthost="--scriptManagers=vw:{},simplecamera:{},js:{--import-paths=../../../../externals/sirikata/liboh/plugins/js/scripts,../../../scripts} --simOptions=ogregraphics:{--search_path=../../../../externals/sirikata/libproxyobject/plugins/}" --object-factory-opts=--db=../../scene.db 
REM --cdn.host=cleopatra.cs.princeton.edu --oh.plugin-search=..\..\Debug --oh.plugins=weight-exp,weight-sqr,tcpsst,oh-file,oh-sqlite,weight-const,ogregraphics,colladamodels,csvfactory,simplecamera,scripting-js,brainvw --objecthost=--scriptManagers=vw:{},simplecamera:{},js:{--import-paths=../../../../externals/sirikata/liboh/plugins/js/scripts} --object-factory-opts=--db=../../scene.db > errlog

REM --default-script-opts=--init-script=../../../../externals/sirikata/liboh/plugins/js/scripts/std/default.em
