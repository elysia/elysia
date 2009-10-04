import Sprite;
import Point;
import Style;

class ButtonControl extends TextButton {
   var mText:TextField;
   var mPressed:Boolean;
   function ButtonControl(parent:MovieClip, lowerLeft:Point, upperRight:Point, text:String) {
     super(parent,lowerLeft,upperRight,text,Style.widgetBorderWidth,Style.widgetLineColor,Style.widgetFillColor,"");
   }
}