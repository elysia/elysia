import ButtonControl;
import Drawable;
import Brain

class MakeLobeButton extends ButtonControl{
    
    function MakeLobeButton(mc:MovieClip,ul:Point,lr:Point,Mode:ModeToggleButton){
    super(mc,ul,lr,"Make a Lobe");
    mMode = Mode;
    }
    var mMode;
    function onPress():Void {
    Brain.makenewlobe();
    mMode.setdrag();
    }
}