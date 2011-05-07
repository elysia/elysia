set DEBDEST=brain\build\sirikata\Debug\
set RELDEST=brain\build\sirikata\Release\

copy externals\sirikata\dependencies\boost_1_44_0\lib\*.dll %DEBDEST%
copy externals\sirikata\dependencies\boost_1_44_0\lib\*.dll %RELDEST%

copy externals\sirikata\dependencies\berkelium\bin\*.* %DEBDEST%
copy externals\sirikata\dependencies\berkelium\bin\*.* %RELDEST%
mkdir %DEBDEST%\locales
mkdir %RELDEST%\locales
copy externals\sirikata\dependencies\berkelium\bin\locales\*.* %DEBDEST%\locales\
copy externals\sirikata\dependencies\berkelium\bin\locales\*.* %RELDEST%\locales\

copy externals\sirikata\dependencies\libantlr3c-3.1.3\bin\*.* %DEBDEST%
copy externals\sirikata\dependencies\libantlr3c-3.1.3\bin\*.* %RELDEST%

copy externals\sirikata\dependencies\bullet-2.74\bin\*.* %DEBDEST%
copy externals\sirikata\dependencies\bullet-2.74\bin\*.* %RELDEST%
copy externals\sirikata\dependencies\ogre-1.7.2\bin\debug\*.* %DEBDEST%
copy externals\sirikata\dependencies\ogre-1.7.2\bin\release\*.* %RELDEST%

copy externals\sirikata\dependencies\installed-curl\*.dll %DEBDEST%
copy externals\sirikata\dependencies\installed-curl\*.dll %RELDEST%
mkdir %DEBDEST%

copy externals\sirikata\dependencies\opencollada\Externals\expat\win32\bin\Debug\*.dll %DEBDEST%
copy externals\sirikata\dependencies\opencollada\Externals\expat\win32\bin\Release\*.dll %RELDEST%

copy externals\sirikata\dependencies\SDL-1.3.0\bin\*.dll %DEBDEST%
copy externals\sirikata\dependencies\SDL-1.3.0\bin\*.dll %RELDEST%

copy externals\sirikata\dependencies\ois-1.0\bin\*.dll %DEBDEST%
copy externals\sirikata\dependencies\ois-1.0\bin\*.dll %RELDEST%
copy externals\sirikata\dependencies\protobufs\bin\*.dll %DEBDEST%
copy externals\sirikata\dependencies\protobufs\bin\*.dll %RELDEST%

copy externals\sirikata\dependencies\installed-gsl\bin\*.dll %DEBDEST%
copy externals\sirikata\dependencies\installed-gsl\bin\*.dll %RELDEST%

mkdir %DEBDEST%\chrome
mkdir %DEBDEST%\chrome\img
mkdir %DEBDEST%\chrome\css
mkdir %DEBDEST%\chrome\js
mkdir %RELDEST%\chrome
mkdir %RELDEST%\chrome\img
mkdir %RELDEST%\chrome\css
mkdir %RELDEST%\chrome\js
copy externals\sirikata\libproxyobject\plugins\ogre\data\chrome\*.* %DEBDEST%\chrome
copy externals\sirikata\libproxyobject\plugins\ogre\data\chrome\*.* %RELDEST%\chrome
copy externals\sirikata\libproxyobject\plugins\ogre\data\chrome\ui\*.* %DEBDEST%\chrome\ui
copy externals\sirikata\libproxyobject\plugins\ogre\data\chrome\ui\*.* %RELDEST%\chrome\ui
copy externals\sirikata\libproxyobject\plugins\ogre\data\chrome\img\*.* %DEBDEST%\chrome\img
copy externals\sirikata\libproxyobject\plugins\ogre\data\chrome\img\*.* %RELDEST%\chrome\img
copy externals\sirikata\libproxyobject\plugins\ogre\data\chrome\js\*.* %DEBDEST%\chrome\js
copy externals\sirikata\libproxyobject\plugins\ogre\data\chrome\js\*.* %RELDEST%\chrome\js
