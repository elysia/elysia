import Sprite;
import Point;

class ButtonControl extends Sprite {
   var mText:TextField;
   var mPressed:Boolean;
   function ButtonControl(parent:MovieClip, lowerLeft:Point, upperRight:Point, text:String) {
     super(parent,parent.getNextHighestDepth());
     var localLowerLeft=lowerLeft.clone();
     var localUpperRight=upperRight.clone();
     parent.globalToLocal(localUpperRight);
     parent.globalToLocal(localLowerLeft);
     var dimensions=localUpperRight.subtractVector(localLowerLeft);
     translateTo(localLowerLeft);
     drawBoxOutline(new Point(0,0),dimensions, 2, 0x7f7f7f, 100, 0x3f3f3f, 50);
     resize(dimensions);
     mSurface.createTextField("mText",mSurface.getNextHighestDepth(),3,3,dimensions.x-6,dimensions.y-6);
     mText=mSurface.mText;
     mText.text=text;

   }
  function translate(worldVector:Point):Void {
     super.translate(worldVector);
     mText._x+=worldVector.x;
     mText._y+=worldVector.y;
  }
  function translateTo(worldPos:Point):Void {
     super.translate(worldPos);
     mText._x=worldPos.x;
     mText._y=worldPos.y;
  }
  function resize(dimensions:Point):Void {
     super.resize(dimensions);
     mText._width=dimensions.x;
     mText._height=dimensions.y;
  }
  function setText(text):Void {
     mText.text=text;
  }  
  function getText():String {
     return mText.text;
  }  
}