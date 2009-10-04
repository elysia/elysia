import Sprite;

class AbstractButton extends Sprite {
  var mFillColor:Number;
  var mLineColor:Number;
  var mSelectedFillColor:Number;
  var mSelectedLineColor:Number;
  function AbstractButton(parent:MovieClip, lowerLeft:Point, upperRight:Point, borderThickness:Number, fillColor:Number, lineColor:Number,image:String, selectedFillColor:Number, selectedLineColor:Number) {
     super(parent,parent.getNextHighestDepth());
     mFillColor=fillColor;
     mLineColor=lineColor;
     var localLowerLeft=lowerLeft.clone();
     var localUpperRight=upperRight.clone();
     parent.globalToLocal(localUpperRight);
     parent.globalToLocal(localLowerLeft);
     var dimensions=localUpperRight.subtractVector(localLowerLeft);
     translateTo(localLowerLeft);
     drawBoxOutline(new Point(0,0),dimensions, borderThickness, fillColor, Style.widgetFillTransparency, lineColor, Style.widgetLineTransparency);
     resize(dimensions);
     if (selectedLineColor==null) {
        mSelectedLineColor=Style.widgetSelectedLineColor;
     }else {
        mSelectedLineColor=selectedLineColor;
     }
     if (selectedFillColor==null) {
        mSelectedFillColor=Style.widgetSelectedFillColor;
     }else {
        mSelectedFillColor=selectedFillColor;
     }
  }
  function select(is_selected:Boolean){
  	clear();
     if(is_selected){
     	//use selected colors
     	drawBoxOutline(new Point(0,0),new Point(this.width,this.height), Style.widgetBorderWidth, mSelectedFillColor, Style.widgetFillTransparency, mSelectedLineColor, Style.widgetLineTransparency);
     }else{
     	drawBoxOutline(new Point(0,0),new Point(this.width,this.height), Style.widgetBorderWidth, mFillColor, Style.widgetFillTransparency, mLineColor, Style.widgetLineTransparency);
     }
  }
}