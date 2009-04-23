class LobeTarget extends Drawable {
  var mVisible:Boolean;
  function LobeTarget(parent:MovieClip,layer:Number){
    super(parent,layer);
    setVisible(false);
  }
  function setSelectedIndex(index:Number):Void {
    setSelectedIndexImpl(index);
    if (index==-1&&!mVisible) {
      this.mSurface._alpha=15; 
    }
    if (index!=-1) {
      this.mSurface._alpha=50; 
    }
  }

  function setVisible(isVisible) {
    if (isVisible) {
      this.mSurface._alpha=50;
      mVisible=true;
    }else {
      if (mIsSelected==-1) {
         this.mSurface._alpha=15;
      }
      mVisible=false;
    }
  }
}