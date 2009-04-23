class SelectionManager {
  static var mSelectedDrawables:Array=new Array();
  static var isDragging:Boolean=false;
  static var sMaxDepth:Number=16383;
  var mSelectBox:Sprite;
  static var sMousePressPos:Point=null;
  static var mLastDragMouseSample:Point;
  static var mLastSelectionPoint=null;
  function SelectionManager(mc:MovieClip) {
     mSelectBox=null;
     var sm:SelectionManager=this;
     mc.onMouseDown=function():Void {
       SelectionManager.sMousePressPos=new Point(_root._xmouse,_root._ymouse)
       if (SelectionManager.mSelectedDrawables.length==0) {
         SelectionManager.selectItem(SelectionManager.sMousePressPos);
       }
       if (SelectionManager.mSelectedDrawables.length!=0) {
         if (Drawable.mMode==Drawable.RESIZEMODE) {
           var i;
           var len=SelectionManager.mSelectedDrawables.length;
           for (i=0;i<len;++i) {
             SelectionManager.mSelectedDrawables[i].onSelectedPress();
           }
         } else if (SelectionManager.mSelectedDrawables.length==1&&!Drawable.isDragMode()) {
           SelectionManager.mSelectedDrawables[0].onSelectedPress();
         }else if (SelectionManager.anyWithin(SelectionManager.sMousePressPos)) {
           SelectionManager.dragSelected();
         }else if (SelectionManager.mSelectedDrawables.length!=0) {
           sm.mSelectBox=new Sprite(mc,SelectionManager.sMaxDepth);
           sm.mSelectBox.setPosition(SelectionManager.sMousePressPos);
         }
       }else {
         sm.mSelectBox=new Sprite(mc,SelectionManager.sMaxDepth);
         sm.mSelectBox.setPosition(SelectionManager.sMousePressPos);
       }
     };
     mc.onMouseUp=function():Void {
        if (SelectionManager.isDragging) {
           SelectionManager.undragSelected();
        }
        var endPoint:Point=new Point(_root._xmouse,_root._ymouse);
        if (Point.distance(SelectionManager.sMousePressPos,endPoint)<Drawable.sMinimumDrawDistance) {
            SelectionManager.selectItem(endPoint);
        }else if (sm.mSelectBox) {
            SelectionManager.selectRegion(SelectionManager.sMousePressPos,endPoint);
            sm.mSelectBox.removeSprite();
        }
        SelectionManager.sMousePressPos=null;
        sm.mSelectBox=null;
     }
     mc.onMouseMove=function() {
        var worldPos=new Point(_root._xmouse,_root._ymouse);
       if (SelectionManager.isDragging) {
          var i;
          var len=SelectionManager.mSelectedDrawables.length;
          for( i=0;i<len;++i) {
             var d:Drawable=SelectionManager.mSelectedDrawables[i];
             d.translate(d.worldToParent(worldPos).subtractVector(d.worldToParent(SelectionManager.mLastDragMouseSample)));
          }
          SelectionManager.mLastDragMouseSample=worldPos;
       }
       if (sm.mSelectBox) {
        var boxColor=0x3f3f3f;
        sm.mSelectBox.clear();
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
  static function anyWithin(where:Point):Boolean {
    var i;
    var len=Brain.mLobes.length;
    for ( i=0;i<len;++i) {
      if(Brain.mLobes[i].within(where)) {  
         return true;
      }
    } 
    len=mSelectedDrawables.length;
    for ( i=0;i<len;++i) {
      if(mSelectedDrawables[i].within(where)) {
         return true;
      }
      var subdrawables=mSelectedDrawables[i].getSubDrawables().length;
      var length=subdrawables.length;
      var j;
      for (j=0;j<length;++j) {
        if (subdrawables[i].within(where)) {
          return true;
        }
      }
    }
   return false;
  }
  static function selectItem(where:Point):Boolean{
    var i:Number;
    var retval:Boolean=false;
    var newSelected:Array=new Array(); 
    var len=Brain.mLobes.length;
    for ( i=0;i<len;++i) {
      if(Brain.mLobes[i].isSelected()==false&&Brain.mLobes[i].within(where)) {
        newSelected.push(Brain.mLobes[i]);
      }
    }

    len=mSelectedDrawables.length;
    for ( i=0;i<len;++i) {
      if(mSelectedDrawables[i].within(where)) {
         newSelected.push(mSelectedDrawables[i]);
      }
      var subdrawables=mSelectedDrawables[i].getSubDrawables().length;
      var length=subdrawables.length;
      var j;
      for (j=0;j<length;++j) {
        if (subdrawables[i].within(where)) {
          newSelected.push(subdrawables[i]);
        }
      }
    }

    clearSelected();//FIXME need to check for double click to change set
    var newlen=newSelected.length;
    for (i=0;i<newlen;++i) {
       addToSelected(newSelected[i]);
       retval=true;
    }
    return retval;
  }
  static function selectRegion(topLeft:Point,bottomRight:Point):Boolean {
    var region:Rect=new Rect(topLeft,bottomRight);
    var retval:Boolean=false;
    var i:Number;
    var newSelected:Array=new Array(); 
    var len=Brain.mLobes.length;
    for ( i=0;i<len;++i) {
      if(Brain.mLobes[i].isSelected()==false&&Brain.mLobes[i].withinShape(region)) {
        newSelected.push(Brain.mLobes[i]);
      }
    }
    len=mSelectedDrawables.length;
    for ( i=0;i<len;++i) {
      if(mSelectedDrawables[i].withinShape(region)) {
         newSelected.push(mSelectedDrawables[i]);
      }
      var subdrawables=mSelectedDrawables[i].getSubDrawables().length;
      var length=subdrawables.length;
      var j;
      for (j=0;j<length;++j) {
        if (subdrawables[i].withinShape(region)) {
          newSelected.push(subdrawables[i]);
        }
      }
    }

    clearSelected();//FIXME need to check for double click to change set
    var newlen=newSelected.length;
    for (i=0;i<newlen;++i) {
       addToSelected(newSelected[i]);
       retval=true;
    }
    return retval;
  }
  static function addToSelected(drawable:Drawable):Void {
    if (drawable.getSelectedIndex()==-1) {
      drawable.setSelectedIndex(mSelectedDrawables.push(drawable)-1);
    }
  }
  static function removeFromSelected(drawable:Drawable):Void{
    var len=mSelectedDrawables.length;
    var i=drawable.getSelectedIndex();
    mSelectedDrawables[i]=mSelectedDrawables[len];
    mSelectedDrawables[i].setSelectedIndex(i);
    drawable.setSelectedIndex(-1);    
  }
  static function clearSelected():Void {
    var len=mSelectedDrawables.length;
    var i;
    for (i=len-1;i>=0;--i) {
       removeFromSelected(mSelectedDrawables[i]);       
    }
    mSelectedDrawables=new Array();
  }
/* doesnt seem to work for   more than 1 item
  static function dragDrawable(drawable:Drawable):Void {

     drawable.mSurface.startDrag(false,-16384,-16384,16384,16384);
  }
*/
  static function dragSelected():Void {
    isDragging=true;
    mLastDragMouseSample=new Point(_root._xmouse,_root._ymouse);
  }
  static function undragSelected():Void {
    isDragging=false;
    var len=mSelectedDrawables.length;
    var i;
    if (sMousePressPos!=null) {
      var invdelta=sMousePressPos.subtractVector(new Point(_root._xmouse,_root._ymouse));
      var delta=(new Point(_root._xmouse,_root._ymouse)).subtractVector(sMousePressPos);
      for (i=0;i<len;++i) {
          mSelectedDrawables[i].translate(invdelta);
          mSelectedDrawables[i].translateUndoable(delta);
      }
    }
  }
};