
include_files = Core.as Sprite.as Lobe.as Point.as Drawable.as ButtonControl.as MakeLobeButton.as ModeToggleButton.as Rect.as Shape.as SelectionManager.as Brain.as TextButton.as AbstractButton.as
daniel_path = /mtasc/std
andrew_path = /usr/share/mtasc/std
nghia_path  = /cygworks/mtasc/std

all: Elysia.swf Prototype_D.swf Prototype_A.swf Prototype_N.swf

Elysia.swf: ${include_files}
	mtasc -cp ${daniel_path} -cp ${andrew_path} -cp ${nghia_path} -main -header 800:600:30 Core.as -swf Elysia.swf
    
Prototype_D.swf: ${include_files}
	mtasc -cp ${daniel_path} -cp ${andrew_path} -cp ${nghia_path} -main -header 800:600:30 Core_D.as -swf Prototype_D.swf
    
Prototype_A.swf: ${include_files}
	mtasc -cp ${daniel_path} -cp ${andrew_path} -cp ${nghia_path} -main -header 800:600:30 Core_A.as -swf Prototype_A.swf
    
Prototype_N.swf: ${include_files}
	mtasc -cp ${daniel_path} -cp ${andrew_path} -cp ${nghia_path} -main -header 800:600:30 Core_N.as -swf Prototype_N.swf