import ButtonControl;
import Drawable;

class MakeLobeButton extends ButtonControl{
    
    function MakeLobeButton(mc:MovieClip,ul:Point,lr:Point){
    super(mc,ul,lr,"Make a Lobe");
    }
    function onPress():Void {
    var newmov=(new Lobe(_root,_root.getNextHighestDepth()));
    var topleft = new Point(10,10);
    var botright = new Point(110,110);
    newmov.commitBox(topleft, botright, 0);
    }
}