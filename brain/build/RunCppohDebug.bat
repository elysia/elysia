cd sirikata
cd Debug
cppoh_d.exe --cdn.host=cleopatra.cs.princeton.edu --oh.plugin-search=..\..\Debug --oh.plugins=weight-exp,weight-sqr,tcpsst,sqlite,weight-const,ogregraphics,colladamodels,csvfactory,simplecamera,scripting-js,brainvw --objecthost=--scriptManagers=vw:{},simplecamera:{},js:{--import-paths=../../../../externals/sirikata/liboh/plugins/js/scripts} --object-factory-opts=--db=../../scene.db > errlog

REM --default-script-opts=--init-script=../../../../externals/sirikata/liboh/plugins/js/scripts/std/default.em
