import Drawable;
import Lobe;
import Core;

class Brain{
    static var brain = new Array();
    var mSelectionManager:SelectionManager;
    static function makenewlobe(){
      var newmov=(new Lobe(_root,_root.getNextHighestDepth()));
      var topleft = new Point(100,40);
      var botright = new Point(200,140);
      newmov.commitBox(topleft, botright, 0);
    }
    function Brain (root_mc:MovieClip) {
       mSelectionManager = new SelectionManager(root_mc);
    }
}