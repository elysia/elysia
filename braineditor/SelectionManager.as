class SelectionManager {
  static var mSelectedDrawables:Array=new Array();
  static var isDragging:Boolean=false;
  static var sMaxDepth:Number=16383;
  var mSelectBox:Sprite=null;
  var mMousePressPos:Point;

  function SelectionManager(mc:MovieClip) {
     var sm:SelectionManager=this;
     mc.mouseDown=function():Void {
       sm.mMousePressPos=new Point(_root._xmouse,_root._ymouse)
       if (SelectionManager.mSelectedDrawables.size()&& !Drawable.isDragMode()) {
         SelectionManager.mSelectedDrawables[0].onPress();
       }else {
         sm.mSelectBox=new Sprite(mc,SelectionManager.sMaxDepth);
         sm.mSelectBox.setPosition(sm.mMousePressPos);
       }
     };
     mc.mouseUp=function():Void {
        var endPoint:Point=new Point(_root._xmouse,_root._ymouse);
        if (Point.distance(sm.mMousePressPos)<Drawable.sMinimumDrawDistance) {
            SelectionManager.selectItem(endPoint);
        }else if (sm.mSelectBox) {
            SelectionManager.selectRegion(sm.mMousePressPos,endPoint);
            sm.mSelectBox.remove();
        }
        sm.mMousePressPos=null;
        sm.mSelectBox=null;
     }
     mc.onMouseMove=function() {
       if (sm.mSelectBox) {
        var boxColor=0x3f3f3f;
        sm.mSelectBox.clear();
        var worldPos=new Point(_root._xmouse,_root._ymouse);
        sm.mSelectBox.drawBoxOutline(new Point(0,0),
                              sm.mSelectBox.worldToLocal(new Point(_root._xmouse,_root._ymouse)),
                              1,
                              boxColor,
                              100,
                              boxColor,
                              25);
         
       }
     }
  }
  static function selectItem(where:Point){

  }
  static function selectRegion(topLeft:Point,bottomRight:Point) {
     
  }
  static function addToSelected(drawable:Drawable):Void {
    if (drawable.getSelectedIndex()==-1) {
      drawable.setSelectedIndex(mSelectedDrawables.push(drawable)-1);
      dragDrawable(drawable);
    }
  }
  static function removeFromSelected(drawable:Drawable):Void{
    var len=mSelectedDrawables.length;
    var i=drawable.getSelectedIndex();
    mSelectedDrawables[i]=mSelectedDrawables[len];
    mSelectedDrawables[i].setSelectedIndex(i);
    drawable.setSelectedIndex(-1);    
    if (isDragging) {
       drawable.mSurface.stopDrag();
    }
  }
  static function clearSelected():Void {
    var len=mSelectedDrawables.length;
    var i;
    for (i=len-1;i>=0;--i) {
       removeFromSelected(mSelectedDrawables[i]);       
    }
    mSelectedDrawables=new Array();
  }
  static function dragDrawable(drawable:Drawable):Void {
     drawable.mSurface.startDrag(false,-16384,-16384,16384,16384);
  }
  static function dragSelected():Void {
    var len=mSelectedDrawables.length;
    var i;
    for (i=0;i<len;++i) {
      dragDrawable(mSelectedDrawables[i]);
    }
  }
  static function undragSelected():Void {
    var len=mSelectedDrawables.length;
    var i;
    for (i=0;i<len;++i) {
        mSelectedDrawables[i].stopDrag();
    }
  }
};