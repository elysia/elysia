import flash.display.*;
import flash.events.*;
import Sprite;
import Point;
import ScrollControl;

class Scroll extends ScrollControl {
    var push_up : ScrollControl;
    function Scroll(parent:MovieClip, lowerLeft:Point, upperRight:Point) {
    	push_up = new ScrollControl(parent,lowerLeft,upperRight);
    	//push_up.translate(new Point(100,200));
    }

}