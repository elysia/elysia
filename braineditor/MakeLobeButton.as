import ButtonControl;
import Drawable;

class MakeLobeButton extends ButtonControl{
    
    function MakeLobeButton(mc:MovieClip,ul:Point,lr:Point,Mode:ModeToggleButton){
    super(mc,ul,lr,"Make a Lobe");
    mMode = Mode;
    }
    var mMode;
    function onPress():Void {
    mMode.setdrag();
    var newmov=(new Lobe(_root,_root.getNextHighestDepth()));
    var topleft = new Point(100,40);
    var botright = new Point(200,140);
    newmov.commitBox(topleft, botright, 0);
    }
}