import Sprite;

class AbstractButton extends Sprite {
  function AbstractButton(parent:MovieClip, lowerLeft:Point, upperRight:Point, borderThickness:Number, fillColor:Number, lineColor:Number,image:String) {
     super(parent,parent.getNextHighestDepth());
     var localLowerLeft=lowerLeft.clone();
     var localUpperRight=upperRight.clone();
     parent.globalToLocal(localUpperRight);
     parent.globalToLocal(localLowerLeft);
     var dimensions=localUpperRight.subtractVector(localLowerLeft);
     translateTo(localLowerLeft);
     drawBoxOutline(new Point(0,0),dimensions, borderThickness, fillColor, 100, lineColor, 50);
     resize(dimensions);
  }
  function select(is_selected:Boolean){
  	clear();
     if(is_selected){
     	//use selected colors
     	drawBoxOutline(new Point(0,0),new Point(this.width,this.height), 2, 0xffcc00, 100, 0xffff00, 50);
     }else{
     	drawBoxOutline(new Point(0,0),new Point(this.width,this.height), 2, 0x7f0000, 100, 0x3f0000, 50);
     }
  }
}