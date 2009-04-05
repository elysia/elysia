import ButtonControl;
import Drawable;

class ModeToggleButton extends ButtonControl{
    
    function ModeToggleButton(mc:MovieClip,ul:Point,lr:Point){
    super(mc,ul,lr,"Drag Mode");
    status=Drawable.DRAGMODE;
    }
    var status;
      function onPress():Void {
        if(status == Drawable.DRAGMODE){
            Drawable.mMode= Drawable.DRAWMODE;
            setText("Draw Mode");
            status=Drawable.DRAWMODE;
        }
        else if(status == Drawable.DRAWMODE){
            setText("Erase Mode");
            Drawable.mMode=Drawable.ERASEMODE;
            status = Drawable.ERASEMODE;
        }
        else if(status == Drawable.ERASEMODE){
            setText("Drag Mode");
            Drawable.mMode=Drawable.DRAGMODE;
            status = Drawable.DRAGMODE;
        }
      }
}
