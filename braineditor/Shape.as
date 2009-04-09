import Gene;
class Shape extends Gene {
   function cutOutShape(deleteShape):Void{}
   function typeOf():String { return "Shape";}
   function isValid():Boolean { return true;}
   function within(point:Point):Boolean {return false;}
}
