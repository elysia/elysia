class SelectionManager {
  static var mSelectedDrawables:Array=new Array();
  static var isDragging:Boolean=false;
  static void addToSelected(drawable:Drawable) {
    if (drawable.getSelectedIndex()==-1) {
      drawable.setSelectedIndex(mSelectedDrawables.push(drawable)-1);
      dragDrawable(drawable);
    }
  }
  static void removeFromSelected(drawable:Drawable) {
    var len=mSelectedDrawables.length;
    var i=drawable.getSelectedIndex();
    mSelectedDrawables[i]=mSelectedDrawables[len];
    mSelectedDrawables[i].setSelectedIndex(i);
    drawable.setSelectedIndex(-1);    
    if (isDragging) {
       drawable.stopDrag();
    }
  }
  static void clearSelected() {
    var len=mSelectedDrawables.length;
    var i;
    for (i=len-1;i>=0;--i) {
       removeFromSelected(mSelectedDrawables[i]);       
    }
    mSelectedDrawables=Array();
  }
  static void dragDrawable(drawable:Drawable) {
     drawable.startDrag(false,-16384,-16384,16384,16384);
  }
  static void dragSelected() {
    var len=mSelectedDrawables.length;
    var i;
    for (i=0;i<len;++i) {
      dragDrawable(mSelectedDrawables[i]);
    }
  }
  static void undragSelected() {
    var len=mSelectedDrawables.length;
    var i;
    for (i=0;i<len;++i) {
        mSelectedDrawables[i].stopDrag();
    }
  }
};