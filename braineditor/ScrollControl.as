import flash.display.*;
import flash.events.*;
import Sprite;
import Point;

class ScrollControl extends AbstractButton {
    function ScrollControl(parent:MovieClip, lowerLeft:Point, upperRight:Point) {
        super(parent,lowerLeft,upperRight,2,0x7f7f7f,0x3f3f3f,"");
        var localLowerLeft=lowerLeft.clone();
        var localUpperRight=upperRight.clone();
        parent.globalToLocal(localUpperRight);
        parent.globalToLocal(localLowerLeft);
        var dimensions=localUpperRight.subtractVector(localLowerLeft);
        translateTo(localLowerLeft);
        drawBoxOutline(new Point(0,0),dimensions, 2, 0x7f0000, 100, 0x3f0000, 50);
        resize(dimensions);
        //Generate 3 boxes
            //top box
            //scroll grab box
            //bottom box
    }
    function onPress(){
    	select(true);
    }
    function onRelease(){
    }
	var mPressed:Boolean;
    var mValue:Number;
    var myMax:Number;
    var myMin:Number;
    //top box actions
    //middle box actions
    //bottom box actions
    
    //Should manipulate object translation
   
}