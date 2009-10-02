class Drawable extends Sprite{
  static var DRAGMODE=0;
  static var DRAWMODE=1;
  static var RESIZEMODE=2;
  static var ERASEMODE=3;
  static var mMode:Number=DRAGMODE;
  static var sMinimumDrawDistance:Number=2;
  var mSelectBox:Sprite;
  ///mIsSelected is -1 if this item is not selected and otherwise the # in the SelectionManagerArray
  var mIsSelected:Number;
  var mShapes:Array;
  static function isDragMode():Boolean {
     return mMode==DRAGMODE;
  }
  function getSubDrawables():Array {
    return new Array();
  }
  function canMakeLobeTarget():Boolean {
    return false;
  }
  function makeLobeTarget():Drawable {
    return null;
  }
  function isSelected():Boolean {
    return mIsSelected!=-1;
  }
  function getSelectedIndex():Number {
     return mIsSelected;
  }
  function setSelectedIndex(index:Number):Void {
    setSelectedIndexImpl(index);
  }
  function setSelectedIndexImpl(index:Number):Void {
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
     var sprite:Drawable=this;
     var spriteTransform:SpriteTransform=new SpriteTransform();
     spriteTransform.save(this);
     UndoManager.performedAction(function() {spriteTransform.save(sprite);sprite.removeSprite();}, function(){sprite.addSprite();sprite.refresh();spriteTransform.restore(sprite);/*_root.tf.text="added "+spriteTransform.toString();*/});
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
  function translateToUndoable(p:Point):Void {
    var sprite:Sprite=this;
    var oldPos=new Point(x,y);
    UndoManager.performedAction(function(){sprite.translateToBase(oldPos);},function(){sprite.translateToBase(p);});
    translateToBase(p);
  }
  function translateUndoable(v:Point):Void {
    var sprite:Sprite=this;
    var oldPos=new Point(-v.x,-v.y);
    UndoManager.performedAction(function(){sprite.translateBase(oldPos);},function(){sprite.translateBase(v);});
    translateBase(v);
  }
  
  function within(p:Point) :Boolean{
     var shapeLength=mShapes.length;
     var i;
     p=worldToLocal(p);
     for (i=0;i<shapeLength;++i) {
       if (mShapes[i].within(p)) {
          return 1==1;
       }
     }
     return 1!=1;
  }
  function withinShape(s:Rect) :Boolean{
     var shapeLength=mShapes.length;
     var i;
     var r=new Rect(worldToLocal(s.mUpperLeft),worldToLocal(s.mLowerRight));
     for (i=0;i<shapeLength;++i) {
       if (mShapes[i].withinShape(r)) {
          return 1==1;
       }
     }
     return 1!=1;
  }
  function commitBox(topLeft:Point, botRight:Point, doErase:Boolean) :Void{
    var rect:Rect=new Rect(topLeft,botRight);
    if (mMode == ERASEMODE){
       var i;
       var shapeLength=mShapes.length;
       var newShapes=new Array();
       for (i=0;i<shapeLength;++i) {
         var shape=mShapes[i];
         var a=shape.cutOutShape(rect);
         var j;
         var aLength=a.length;
         for (j=0;j<aLength;++j) {
            newShapes.push(a.pop());
         }
       }
       var drawable:Drawable=this;
       var oldShapes=mShapes;
       UndoManager.performedAction(function(){drawable.mShapes=oldShapes;drawable.refresh();},
                                   function(){drawable.mShapes=newShapes;drawable.refresh();});
       mShapes=newShapes;
       refresh();
    }else {
      mShapes.push(rect);
      drawRect(rect);
      var drawable:Drawable=this;
      UndoManager.performedAction(function(){drawable.mShapes.pop();drawable.refresh();},
                                  function(){drawable.mShapes.push(rect);drawable.drawRect(rect);});

    }
  }
  function nonundoResize(delta:Point):Boolean{
     var len:Number=mShapes.length;
     var i:Number;
     var topLeft:Point;
     var bottomRight:Point;
     for (i=0;i<len;++i) {
        var br:Point=mShapes[i].bottomRightCorner();
        var tl:Point=mShapes[i].topLeftCorner();
        if (topLeft) {
           topLeft=topLeft.minimum(tl);
           bottomRight=bottomRight.maximum(br);
        }else{
           bottomRight=br.clone();
           topLeft=tl;
        }
     }
     //_root.tf.text="Scaling by "+bottomRight.toString()+" "+topLeft.toString();
     if (bottomRight) {
        if (topLeft) {
           var newBottomRight:Point=new Point(bottomRight.x+delta.x,bottomRight.y+delta.y); 
           if (bottomRight.x-topLeft.x>.01 || topLeft.x-bottomRight.x>.01) {}else {
              bottomRight.x=topLeft.x+.1;
           }
           if (bottomRight.y-topLeft.y>.01 || topLeft.y-bottomRight.y>.01) {}else {
              bottomRight.y=topLeft.y+.1;
           }
           var scale=new Point(newBottomRight.subtractVector(topLeft).x/bottomRight.subtractVector(topLeft).x,
                               newBottomRight.subtractVector(topLeft).y/bottomRight.subtractVector(topLeft).y);
           if (scale.x>.01&&scale.x<10&&scale.y>.01&&scale.y<10) {
              var shapes=mShapes;
              mShapes=new Array();
              for (i=0;i<len;++i) {                 
                  mShapes.push(shapes[i].scale(topLeft,scale));
              }
              return true;
           }
        }
     }
     return false;
  }
  function undoableResize(point:Point):Boolean{
     var shapes:Array=new Array();
     var drawable:Drawable=this;
     var oldShapes=mShapes;
     var retval:Boolean=nonundoResize(point);
     var newShapes=mShapes;
     UndoManager.performedAction(function(){drawable.mShapes=oldShapes;drawable.refresh();},function(){drawable.mShapes=newShapes;drawable.refresh();});
     return retval;
  }
  function onSelectedPress():Void {
      if (mMode==RESIZEMODE) {
         var drawable=this;
         var mousePos=new Point(_root._xmouse,_root._ymouse)
         var localMousePos=worldToLocal(mousePos);
         var lastMousePos=worldToLocal(mousePos);
         mSurface.onMouseMove=function() {
           var worldCurMousePos=new Point(_root._xmouse,_root._ymouse)
           var curMousePos=drawable.worldToLocal(worldCurMousePos);
           
           if (drawable.nonundoResize(localMousePos.subtractVector(lastMousePos))) {
             drawable.nonundoResize(curMousePos.subtractVector(localMousePos));
           }
           drawable.refresh();
           lastMousePos.x=curMousePos.x;
           lastMousePos.y=curMousePos.y;
         }
         mSurface.onMouseUp=function() {
           drawable.mSurface.onMouseUp=function(){};
           drawable.mSurface.onMouseMove=function(){};
           var worldCurMousePos=new Point(_root._xmouse,_root._ymouse)
           var curMousePos=drawable.worldToLocal(worldCurMousePos);
           if (drawable.nonundoResize(localMousePos.subtractVector(lastMousePos))) {
             drawable.undoableResize(curMousePos.subtractVector(localMousePos));
           }
           drawable.refresh();
           //_root.tf.text="resizeDONE "+curMousePos.subtractVector(localMousePos).toString();
         }
      }else if (mMode==DRAWMODE||mMode==ERASEMODE) {
         var mousePos=new Point(_root._xmouse,_root._ymouse)
         mSelectBox=new Sprite(mSurface,mSurface.getNextHighestDepth());
         var localMousePos=worldToLocal(mousePos);
         mSelectBox.setPosition(localMousePos);
         mSelectBox.penTo(mousePos);
         var drawable=this;
         var doErase=(mMode==ERASEMODE);
         var tempBoxColor=0x0000ff;
         if (doErase) {
            //mMode=DRAWMODE;///<-- FIXME: this alternates between draw and erase modes until we have global selection process
            tempBoxColor=0xff0000;
         }else { 
            //mMode=ERASEMODE;
         }
         mSurface.onMouseMove=function(){drawable._mouseFunction(tempBoxColor);};
         mSurface.onMouseUp=function()
         {
            drawable.mSurface.onMouseUp=function(){};
            drawable.mSurface.onMouseMove=function(){};
            var endPoint:Point=drawable.worldToLocal(new Point(_root._xmouse,_root._ymouse));
            if (Point.distance(endPoint,drawable.mSelectBox.getPosition())>=Drawable.sMinimumDrawDistance) {
              drawable.commitBox(drawable.mSelectBox.getPosition(),endPoint,doErase);
            }
            drawable.mSelectBox.removeSprite();
            drawable.mSelectBox=null;
         }
     }
  }
  function onRelease():Void {
  }
}
