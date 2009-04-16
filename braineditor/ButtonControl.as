import Sprite;
import Point;

class ButtonControl extends TextButton {
   var mText:TextField;
   var mPressed:Boolean;
   function ButtonControl(parent:MovieClip, lowerLeft:Point, upperRight:Point, text:String) {
     super(parent,lowerLeft,upperRight,text,2,0x7f7f7f,0x3f3f3f,"");
   }
}