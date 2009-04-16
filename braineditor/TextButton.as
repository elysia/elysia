import Sprite;
import Point;
import AbstractButton
class TextButton extends AbstractButton {
   var mText:TextField;
   var mPressed:Boolean;
   function TextButton(parent:MovieClip, lowerLeft:Point, upperRight:Point, text:String, borderThickness:Number, fillColor:Number, lineColor:Number,image:String) {
     super(parent,lowerLeft,upperRight,borderThickness,fillColor,lineColor,image);
     var localLowerLeft=lowerLeft.clone();
     var localUpperRight=upperRight.clone();
     parent.globalToLocal(localUpperRight);
     parent.globalToLocal(localLowerLeft);
     var dimensions=localUpperRight.subtractVector(localLowerLeft);;
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