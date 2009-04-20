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
     drawBoxOutline(new Point(0,0),dimensions, 2, 0x7f7f7f, 100, 0x3f3f3f, 50);
     resize(dimensions);
  }
  function select(is_selected:Boolean){
  	clear();
     if(is_selected){
     	drawBoxOutline(new Point(0,0),new Point(this.width,this.height), 2, 0x00007f, 100, 0x00003f, 50);
     }
  }
}