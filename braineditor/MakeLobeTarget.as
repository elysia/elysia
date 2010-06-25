import ButtonControl;
import Drawable;
import Brain;

class MakeLobeTarget extends ButtonControl{
    
    function MakeLobeTarget(mc:MovieClip,ul:Point,lr:Point,Mode:ModeToggleButton){
      super(mc,ul,lr,"Make Lobe Target");
    }
    function onPress():Void {
      Brain.makeLobeTarget();
    }
}