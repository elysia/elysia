class Drawable extends Sprite{
  static var DRAGMODE=0;
  static var DRAWMODE=1;
  static var ERASEMODE=2;
  static var mMode:Number;
  var mSelectBox:Sprite;
  var mMouseCount;
  function Drawable(parent:MovieClip,layer:Number) {
     super(parent,layer);
     mMode=DRAGMODE;
     //mMode=DRAGMODE;
     //mMode=ERASEMODE;
  }
  function _mouseFunction(boxColor:Number) {
         mSelectBox.clear();
         var worldPos=new Point(_root._xmouse,_root._ymouse);
         mSelectBox.drawBoxOutline(mSelectBox.localToWorld(new Point(0,0)),
                                   new Point(_root._xmouse,_root._ymouse),
                                   1,
                                   boxColor,
                                   100,
                                   boxColor,
                                   25);
  }
  function commitBox(topLeft:Point, botRight:Point, doErase:Boolean) {
    if (mMode == ERASEMODE){
    ///FIXME we probably want to call refresh and redraw everything so that a doughnut with a hole in the middle isn't actually a round bread with a painted white in the middle
      drawBoxOutline(topLeft,botRight,
                                     1,
                                     0xffffff,
                                     0,
                                     0xffffff,
                                     100);
    }else {
      drawBoxOutline(topLeft,botRight,
                                     1,
                                     0xffffff,
                                     0,
                                     0x00ffff,
                                     25);
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
            lobe.commitBox(lobe.localToWorld(lobe.mSelectBox.getPosition()),new Point(_root._xmouse,_root._ymouse),doErase);
            lobe.mSelectBox.remove();
            lobe.mSelectBox=null;
         }
     }
  }
  function onRelease():Void {
  }
}