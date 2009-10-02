import flash.display.*;
import flash.events.*;
import Sprite;
import Point;
import ScrollControl;

class Scroll extends ScrollControl {
    var push_up : ScrollControl;
    var push_down : ScrollControl;
    function Scroll(parent:MovieClip, lowerLeft:Point, upperRight:Point) {
        super(parent,lowerLeft,upperRight)
    	push_up = new ScrollControl(mSurface,lowerLeft,upperRight);
    	push_down = new ScrollControl(mSurface,lowerLeft.addVector(new Point(0,80)),upperRight.addVector(new Point(0,80)));
    }

}