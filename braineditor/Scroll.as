import flash.display.*;
import flash.events.*;
import Sprite;
import Point;
import ScrollControl;

class Scroll extends Sprite {
    var push_up : AbstractButton;
    var push_down : AbstractButton;
    function Scroll(parent:MovieClip, lowerLeft:Point, upperRight:Point) {
        super(parent)
    	push_up = new AbstractButton(mSurface,lowerLeft,upperRight,1,0x007f7f,0x7f7f00,"upArrow.png");
    	push_down = new AbstractButton(mSurface,lowerLeft.addVector(new Point(0,80)),upperRight.addVector(new Point(0,80)),1,0x007f7f,0x7f7f00,"downArrow.png");

    }

}