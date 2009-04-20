import Drawable;
import Lobe;
import Core;

class Brain{
    static var mLobes:Array = new Array();
    var mSelectionManager:SelectionManager;
    static function makenewlobe(){
      var newmov=(new Lobe(_root,_root.getNextHighestDepth()));
      var topleft = new Point(100,40);
      var botright = new Point(200,140);
      newmov.commitBox(topleft, botright, 0);
      UndoManager.coalesceUndos();
      mLobes.push(newmov);//need to add it to the array of lobes;
    }

    function Brain (root_mc:MovieClip) {
       mSelectionManager = new SelectionManager(root_mc);
       var keyListener = {};
       keyListener.onKeyDown = function()
       {
            var k = Key.getCode();
            if(k == Key.DELETEKEY){
                Brain.makenewlobe();
            }
            if (k== 90) {
                UndoManager.undo();
            }
            if (k== 89) {
                UndoManager.redo();
            }

       };
       Key.addListener( keyListener );
    }
}

