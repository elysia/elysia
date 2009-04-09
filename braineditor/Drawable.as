class Drawable extends Sprite{
  static var DRAGMODE=0;
  static var DRAWMODE=1;
  static var ERASEMODE=2;
  static var mMode:Number=DRAGMODE;
  static var sMinimumDrawDistance:Number=2;
  var mSelectBox:Sprite;
  ///mIsSelected is -1 if this item is not selected and otherwise the # in the SelectionManagerArray
  var mIsSelected:Number;
  var mShapes:Array;
  static function isDragMode():Boolean {
     return mMode==DRAGMODE;
  }
  function isSelected():Boolean {
    return mIsSelected!=-1;
  }
  function getSelectedIndex():Number {
     return mIsSelected;
  }
  function setSelectedIndex(index:Number):Void {
     var oldIndex=mIsSelected;
     mIsSelected=index;
     if (((oldIndex==-1)!=(mIsSelected==-1)) 
        || ((oldIndex==0)!=(mIsSelected==0)))
        refresh();
  }
  function Drawable(parent:MovieClip,layer:Number) {
     super(parent,layer);
     mIsSelected=-1;
     mShapes=new Array();
  }
  function _mouseFunction(boxColor:Number):Void {
    mSelectBox.clear();
    var worldPos=new Point(_root._xmouse,_root._ymouse);
    mSelectBox.drawBoxOutline(new Point(0,0),
                              mSelectBox.worldToLocal(new Point(_root._xmouse,_root._ymouse)),
                              1,
                              boxColor,
                              100,
                              boxColor,
                              25);
  }
  function drawRect(rect:Rect) :Void{
      var color:Number=0x00ffff;
      var alpha:Number=25;
      if (isSelected()) {
         color=0x0025ff;         
      }
      rect.drawBox(this,color,alpha);
  }
  function refresh() :Void{
     clear();
     var shapeLength=mShapes.length;
     var i;
     for (i=0;i<shapeLength;++i) {
       drawRect(mShapes[i]);
     }
  }
  function commitBox(topLeft:Point, botRight:Point, doErase:Boolean) :Void{
    var rect:Rect=new Rect(topLeft,botRight);
    if (mMode == ERASEMODE){
       var newShapes=new Array();
       var i;
       var shapeLength=mShapes.length;
       for (i=0;i<shapeLength;++i) {
         var shape=mShapes.pop();
         var a=shape.cutOutShape(rect);
         var j;
         var aLength=a.length;
         for (j=0;j<aLength;++j) {
            newShapes.push(a.pop());
         }
       }
       mShapes=newShapes;
       refresh();
/*
       ///FIXME we probably want to call refresh and redraw everything so that a doughnut with a hole in the middle isn't actually a round bread with a painted white in the middle
       drawBoxOutline(topLeft,botRight,
                                     1,
                                     0xffffff,
                                     0,
                                     0xffffff,
                                     100);
*/
    }else {
      mShapes.push(rect);
      drawRect(rect);
    }
  }
  function onPress():Void {
     if (mMode==DRAGMODE) {
         mSurface.startDrag(false,-16384,-16384,16384,16834);
         var lobe=this;
         mSurface.onMouseUp=function()
         {
            lobe.mSurface.onMouseUp=function(){};
            lobe.mSurface.stopDrag();
         }
     }else if (mMode==DRAWMODE||mMode==ERASEMODE) {
         var mousePos=new Point(_root._xmouse,_root._ymouse)
         mSelectBox=new Sprite(mSurface,mSurface.getNextHighestDepth());
         var localMousePos=worldToLocal(mousePos);
         mSelectBox.setPosition(localMousePos);
         mSelectBox.penTo(mousePos);
         var lobe=this;
         var doErase=(mMode==ERASEMODE);
         var tempBoxColor=0x0000ff;
         if (doErase) {
            //mMode=DRAWMODE;///<-- FIXME: this alternates between draw and erase modes until we have global selection process
            tempBoxColor=0xff0000;
         }else { 
            //mMode=ERASEMODE;
         }
         mSurface.onMouseMove=function(){lobe._mouseFunction(tempBoxColor);};
         mSurface.onMouseUp=function()
         {
            lobe.mSurface.onMouseUp=function(){};
            lobe.mSurface.onMouseMove=function(){};
            var endPoint:Point=lobe.worldToLocal(new Point(_root._xmouse,_root._ymouse));
            if (Point.distance(endPoint,lobe.mSelectBox.getPosition())>=Drawable.sMinimumDrawDistance) {
              lobe.commitBox(lobe.mSelectBox.getPosition(),endPoint,doErase);
            }
            lobe.mSelectBox.remove();
            lobe.mSelectBox=null;
         }
     }
  }
  function onRelease():Void {
  }
}