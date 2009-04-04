class Drawable extends Sprite{
  static var DRAGMODE=0;
  static var DRAWMODE=1;
  var mMode:Number;
  var mSelectBox:Sprite;
  var mMouseCount;
  function Drawable(parent:MovieClip,layer:Number) {
     super(parent,layer);
     this.mMode=DRAWMODE;
     //this.mMode=DRAGMODE;
  }
  function _mouseFunction() {
         mSelectBox.clear();
         var worldPos=new Point(_root._xmouse,_root._ymouse);
         mSelectBox.drawBox(mSelectBox.localToWorld(new Point(0,0)),new Point(_root._xmouse,_root._ymouse));
  }
  function onPress():Void {
     if (this.mMode==DRAGMODE) {
         mSurface.startDrag(false,-16384,-16384,16384,16834);
         var lobe=this;
         mSurface.onMouseUp=function()
         {
            lobe.mSurface.onMouseUp=function(){};
            lobe.mSurface.stopDrag();
         }
     }else if (this.mMode=DRAWMODE){
         var mousePos=new Point(_root._xmouse,_root._ymouse)
         mSelectBox=new Sprite(mSurface,mSurface.getNextHighestDepth());
         var localMousePos=worldToLocal(mousePos);
         mSelectBox.mSurface._x=localMousePos.x;
         mSelectBox.mSurface._y=localMousePos.y;
         mSelectBox.penTo(mousePos);
         var lobe=this;
         mSurface.onMouseMove=function(){lobe._mouseFunction();};
         mSurface.onMouseUp=function()
         {
            lobe.mSurface.onMouseUp=function(){};
            lobe.mSurface.onMouseMove=function(){};
            lobe.mSelectBox.remove();
            lobe.mSelectBox=null;
         }
     }
  }
  function onRelease():Void {
  }
}