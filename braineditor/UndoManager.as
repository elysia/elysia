class UndoManager {
  //an array of how to undo the recently performed actions
  static var sUndoList:Array=new Array();
  //an array of the recently performed actions
  static var sRedoList:Array=new Array();
  //the current undo number...if it's not the end of the sUndoList, those actions need to be replicated
  static var sCurUndoCounter:Number=0;
  static function coalesceUndos():Void {
    var length:Number=sUndoList.length;
    if (length>1) {
        var lastUndo=sUndoList[length-1];
        var penultimateUndo=sUndoList[length-2];
        var lastRedo=sRedoList[length-1];
        var penultimateRedo=sRedoList[length-2];
        sUndoList.pop();
        sUndoList.pop();
        sRedoList.pop();
        sRedoList.pop();
        performedAction(function(){lastUndo();penultimateUndo();},function(){penultimateRedo();lastRedo();});
    }
  }
  static function performedAction(undoFunction, redoFunction):Void{
    if (sCurUndoCounter!=sUndoList.length) {
      var i:Number;
      var length:Number=sUndoList.length;
      var count:Number=0;
      for (i=sCurUndoCounter;i<length;++i) {
        sUndoList.push(sRedoList[length-1-count]);
        sRedoList.push(sUndoList[length-1-count]);
        ++count;
      }
    }
    sUndoList.push(undoFunction);
    sRedoList.push(redoFunction);
    sCurUndoCounter=sUndoList.length;
  }

  static function undo():Void{      
    if (sCurUndoCounter>0) {
      sCurUndoCounter--;
      sUndoList[sCurUndoCounter]();
    }
  }

  static function redo():Void {
    if (sCurUndoCounter<sUndoList.length) {
      sRedoList[sCurUndoCounter]();
      sCurUndoCounter++;
    }
  }
}
