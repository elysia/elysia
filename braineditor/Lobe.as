class Lobe extends Drawable{
  var mLobeTargets:Array;
  function getSubDrawables():Array {
     return mLobeTargets;
  }
  function canMakeLobeTarget():Boolean { return true;}
  function makeLobeTarget():Drawable {
     var retval:LobeTarget;
     retval=new LobeTarget(mParent,mParent.getNextDepthLayer());
     mLobeTargets.push(retval);
     var lobe:Lobe=this;
     UndoManager.performedAction(function(){lobe.mLobeTargets.pop();},function(){lobe.mLobeTargets.push(retval);});
     UndoManager.coalesceUndos();
     return retval;
  }
  function Lobe(parent:MovieClip,layer:Number) {
     super(parent,layer);
     mLobeTargets=new Array();
     
     //All spotches are stored in mshapes
  }
}
