import flash.display.*;
import flash.events.*;
import Sprite;
import Point;

class ScrollControl extends AbstractButton {
	var mPressed:Boolean;
    var mValue:Number = 1;
    var myMax:Number;
    var myMin:Number;
    var mText:TextField;
    
    function ScrollControl(parent:MovieClip, lowerLeft:Point, upperRight:Point) {
      super(parent,lowerLeft,upperRight,Style.widgetBorderWidth,Style.scrollArrowFillColor,Style.scrollArrowOutlineColor,"");
      var dimensions=upperRight.subtractVector(lowerLeft);
      mSurface.createTextField("mText",mSurface.getNextHighestDepth(),3,3,dimensions.x-6,dimensions.y-6);
      mText=mSurface.mText;
      mText.text=mValue.toString();
            //scroll grab box
            //bottom box
    }
    function onPress(){
    	select(true);
    	mValue++;
    	mText.text=mValue.toString();
    }
    function onRelease(){
    	select(false);
    }
    function onReleaseOutside(){
    	select(false);
    }
    

    
    
    //top box actions
    //middle box actions
    //bottom box actions
    
    //Should manipulate object translation
   
}