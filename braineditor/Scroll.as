import flash.display.*;
import flash.events.*;
import Sprite;
import Point;
import ScrollControl;

class Scroll extends Sprite {
    var push_up : ScrollControl;
    var push_down : ScrollControl;
    function Scroll(parent:MovieClip, upperArrowLowerLeft:Point, upperArrowUpperRight:Point, lowerArrowLowerLeft:Point, lowerArrowLowerRight:Point) {
        super(parent,null,true)
        push_up = new ScrollControl(mSurface,upperArrowLowerLeft, upperArrowUpperRight);
        push_down = new ScrollControl(mSurface,lowerArrowLowerLeft, lowerArrowLowerRight);
    }
}